#include "Stage.hpp"
#include "common.hpp"
#include "draw.hpp"
#include "util.hpp"
#include <cstdlib>
#include <ctime>

using namespace std;

extern bool keyboard[MAX_KEYBOARD_KEYS];

enum class Stage::Side
{
    Player,
    Alien
};

struct Stage::Entity
{
    float x;
    float y;
    int w;
    int h;
    float dx;
    float dy;
    int health;
    int reload; // 8 frames (approx 0.133333 seconds) must pass before we can fire again
    Side side;
    SDL_Texture *texture;
};

struct Stage::Explosion
{
    float x;
    float y;
    float dx;
    float dy;
    int r, g, b, a;
};

struct Stage::Debris
{
    float x;
    float y;
    float dx;
    float dy;
    int life;
    SDL_Rect rect;
    SDL_Texture *texture;
};

Stage::Stage(int highscore,
             SDL_Renderer *renderer,
             std::shared_ptr<AudioPlayer> audio_player,
             std::shared_ptr<Text> text) : player(nullptr),
                                           highscore(highscore),
                                           renderer(renderer),
                                           audio_player(audio_player),
                                           text(text)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    playerTexture = loadTexture(renderer, PLAYER_TEXTURE_PATH);

    bulletTexture = loadTexture(renderer, BULLET_TEXTURE_PATH);

    enemyTexture = loadTexture(renderer, ENEMY_TEXTURE_PATH);

    pointsTexture = loadTexture(renderer, POINTS_TEXTURE_PATH);

    alienBulletTexture = loadTexture(renderer, ALIEN_BULLET_TEXTURE_PATH);

    explosionTexture = loadTexture(renderer, EXPLOSION_TEXTURE_PATH);

    resetStage();
}

Stage::~Stage()
{
    if (player.get() != nullptr)
    {
        SDL_DestroyTexture(player->texture);
    }

    SDL_DestroyTexture(bulletTexture);

    SDL_DestroyTexture(pointsTexture);

    auto it_fighter = enemyList.begin();
    while (it_fighter != enemyList.end())
    {
        if (it_fighter->texture != nullptr)
        {
            SDL_DestroyTexture(it_fighter->texture);
        }
        it_fighter++;
    }

    auto it_bullet = bulletList.begin();
    while (it_bullet != bulletList.end())
    {
        if (it_bullet->texture != nullptr)
        {
            SDL_DestroyTexture(it_bullet->texture);
        }
        it_bullet++;
    }

    SDL_DestroyTexture(enemyTexture);

    SDL_DestroyTexture(alienBulletTexture);
}

void Stage::logic()
{
    doPlayer();

    doEnemies();

    doBullets();

    spawnEnemies();

    doExplosions();

    doDebris();

    doPointsPods();

    clipPlayer();

    if (!player && --stageResetTimer <= 0)
    {
        gameover = true;
    }
}

void Stage::draw()
{
    drawPointsPods();

    drawPlayer();

    drawBullets();

    drawFighters();

    drawDebris();

    drawExplosions();

    drawHud();
}

void Stage::initPlayer()
{
    player.reset(new Entity);
    player->x = 100;
    player->y = 100;
    player->side = Side::Player;
    player->health = 10;
    player->reload = 0;
    player->texture = playerTexture;
    SDL_QueryTexture(player->texture, nullptr, nullptr, &player->w, &player->h);
}

void Stage::resetStage()
{
    enemyList.clear();
    bulletList.clear();
    pointList.clear();
    explosionList.clear();
    debrisList.clear();

    initPlayer();

    score = 0;

    enemySpawnTimer = 0;

    stageResetTimer = FPS * 3;

    gameover = false;
}

void Stage::doPlayer()
{
    if (!player)
        return;

    if (player->health == 0)
    {
        addExplosions(player->x, player->y, rand() % 10 + 10);
        addDebris(player.get());
        player.reset(nullptr);
        return;
    }

    player->dx = player->dy = 0;

    if (player->reload > 0)
    {
        player->reload--;
    }

    if (keyboard[SDL_SCANCODE_UP])
    {
        player->dy = -PLAYER_SPEED;
    }

    if (keyboard[SDL_SCANCODE_DOWN])
    {
        player->dy = PLAYER_SPEED;
    }

    if (keyboard[SDL_SCANCODE_LEFT])
    {
        player->dx = -PLAYER_SPEED;
    }

    if (keyboard[SDL_SCANCODE_RIGHT])
    {
        player->dx = PLAYER_SPEED;
    }

    if (keyboard[SDL_SCANCODE_LCTRL] && player->reload <= 0)
    {
        fireBullet();
        audio_player->playSound(SND_Player_Fire, CH_Player);
    }

    player->x += player->dx;
    player->y += player->dy;
}

void Stage::doBullets()
{
    auto it = bulletList.begin();
    while (it != bulletList.end())
    {
        it->x += it->dx;
        it->y += it->dy;

        if (bulletHitEnemy(it) ||
            bulletHitPlayer(it) ||
            it->x > SCREEN_WIDTH ||
            it->y > SCREEN_HEIGHT ||
            it->x < -it->w ||
            it->y < -it->h)
        {
            it = bulletList.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Stage::doEnemies()
{
    auto it = enemyList.begin();
    while (it != enemyList.end())
    {
        it->x += it->dx;

        // avoid enemies going across upper/lower boundary
        if (it->y >= SCREEN_HEIGHT - it->h || it->y <= 0)
        {
            it->dy = -it->dy;
        }
        it->y += it->dy;

        // erase out of boundary enemies
        if (it->x < -it->w || it->health == 0 || enemyHitPlayer(it))
        {
            it = enemyList.erase(it);
        }
        else
        {
            if (player.get() != nullptr && --it->reload <= 0)
            {
                fireAlienBullet(it);
                audio_player->playSound(SND_Alien_Fire, CH_Alien_Fire);
            }
            it++;
        }
    }
}

void Stage::doExplosions()
{
    auto it = explosionList.begin();
    while (it != explosionList.end())
    {
        it->x += it->dx;
        it->y += it->dy;

        if (--it->a <= 0)
        {
            it = explosionList.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Stage::doDebris()
{
    auto it = debrisList.begin();
    while (it != debrisList.end())
    {
        it->x += it->dx;
        it->y += it->dy;
        it->dy += 0.5;

        if (--it->life <= 0)
        {
            it = debrisList.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Stage::doPointsPods()
{
    auto it = pointList.begin();
    while (it != pointList.end())
    {
        if (it->x < 0)
        {
            it->x = 0;
            it->dx = -it->dx;
        }

        if (it->x + it->w > SCREEN_WIDTH)
        {
            it->x = SCREEN_WIDTH - it->w;
            it->dx = -it->dx;
        }

        if (it->y < 0)
        {
            it->y = 0;
            it->dy = -it->dy;
        }

        if (it->y + it->h > SCREEN_HEIGHT)
        {
            it->y = SCREEN_HEIGHT - it->h;
            it->dy = -it->dy;
        }

        it->x += it->dx;
        it->y += it->dy;

        if (player.get() != nullptr && collision(it->x, it->y, it->w, it->h, player->x, player->y, player->w, player->h))
        {
            it->health = 0;
            score++;
            highscore = max(highscore, score);
            audio_player->playSound(SND_POINTS, CH_POINTS);
        }

        if (--it->health <= 0)
        {
            it = pointList.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Stage::fireBullet()
{
    Entity bullet;

    bullet.x = player->x;
    bullet.y = player->y;
    bullet.dx = PLAYER_BULLET_SPEED;
    bullet.health = 1;
    bullet.texture = bulletTexture;
    SDL_QueryTexture(bullet.texture, nullptr, nullptr, &bullet.w, &bullet.h);

    bullet.y += (player->h / 2) - (bullet.h / 2);

    bullet.side = Side::Player;

    bulletList.emplace_back(move(bullet));

    player->reload = 8;
}

void Stage::fireAlienBullet(EntityIt enemy)
{
    Entity bullet;

    bullet.x = enemy->x;
    bullet.y = enemy->y;
    bullet.health = 1;
    bullet.texture = alienBulletTexture;
    bullet.side = Side::Alien;
    SDL_QueryTexture(bullet.texture, nullptr, nullptr, &bullet.w, &bullet.h);

    bullet.x += (enemy->w / 2) - (bullet.w / 2);
    bullet.y += (enemy->h / 2) - (bullet.h / 2);

    calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), enemy->x, enemy->y, bullet.dx, bullet.dy);

    bullet.dx *= ALIEN_BULLET_SPEED;
    bullet.dy *= ALIEN_BULLET_SPEED;

    bulletList.emplace_back(move(bullet));

    enemy->reload = (rand() % FPS * 2);
}

bool Stage::bulletHitEnemy(EntityIt bullet)
{
    for (auto it = enemyList.begin(); it != enemyList.end(); it++)
    {
        if (bullet->side != it->side &&
            collision(bullet->x, bullet->y, bullet->w, bullet->h, it->x, it->y, it->w, it->h))
        {
            addPointsPod(it->x + it->w / 2, it->y + it->h / 2);
            audio_player->playSound(SND_Alien_Die, CH_Any); // play multiple explosion sounds at the same time
            score++;
            highscore = max(highscore, score);
            bullet->health = 0;
            it->health--;
            if (it->health == 0)
            {
                addExplosions(it->x, it->y, rand() % 10 + 5);
                addDebris(&(*it));
            }
            return true;
        }
    }
    return false;
}

bool Stage::bulletHitPlayer(EntityIt bullet)
{
    if (!player)
        return false;

    if (bullet->side != player->side &&
        collision(bullet->x, bullet->y, bullet->w, bullet->h, player->x, player->y, player->w, player->h))
    {
        audio_player->playSound(SND_Player_Die, CH_Player);
        bullet->health = 0;
        player->health--;
        return true;
    }
    return false;
}

bool Stage::enemyHitPlayer(EntityIt enemy)
{
    if (!player)
        return false;

    if (collision(player->x, player->y, player->w, player->h, enemy->x, enemy->y, enemy->w, enemy->h))
    {
        player->health--;
        enemy->health = 0;
        addExplosions(enemy->x, enemy->y, rand() % 10 + 5);
        addDebris(&(*enemy));
        return true;
    }
    return false;
}

void Stage::addExplosions(int x, int y, int num)
{
    for (size_t i = 0; i < num; i++)
    {
        Explosion e;

        // explosion position and speed
        e.x = x + (rand() % 32) - (rand() % 32);
        e.y = y + (rand() % 32) - (rand() % 32);
        e.dx = (rand() % 10) - (rand() % 10);
        e.dy = (rand() % 10) - (rand() % 10);
        e.dx /= 10;
        e.dy /= 10;

        // explosion color
        switch (rand() % 4)
        {
        case 0:
            e.r = 255;
            break;

        case 1:
            e.r = 255;
            e.g = 128;
            break;

        case 2:
            e.r = 255;
            e.g = 255;
            break;

        default:
            e.r = 255;
            e.g = 255;
            e.b = 255;
            break;
        }

        // explosion lifetime
        e.a = rand() % FPS * 3;

        explosionList.emplace_back(move(e));
    }
}

void Stage::addDebris(Entity *e)
{
    int x, y, w, h;

    w = e->w / 2;
    h = e->h / 2;

    for (y = 0; y <= h; y += h)
    {
        for (x = 0; x <= w; x += w)
        {
            Debris d;

            d.x = e->x + e->w / 2;
            d.y = e->y + e->h / 2;
            d.dx = (rand() % 5) - (rand() % 5);
            d.dy = -(5 + rand() % 12);
            d.life = FPS * 2;
            d.texture = e->texture;
            d.rect.x = x;
            d.rect.y = y;
            d.rect.w = w;
            d.rect.h = h;

            debrisList.emplace_back(move(d));
        }
    }
}

void Stage::addPointsPod(int x, int y)
{
    Entity point;
    point.x = x;
    point.y = y;
    point.dx = -(rand() % 5);
    point.dy = (rand() % 5) - (rand() % 5);
    point.health = FPS * 10;
    point.texture = pointsTexture;

    SDL_QueryTexture(point.texture, nullptr, nullptr, &point.w, &point.h);

    point.x -= point.w / 2;
    point.y -= point.h / 2;

    pointList.emplace_back(move(point));
}

void Stage::spawnEnemies()
{
    if (--enemySpawnTimer <= 0)
    {
        Entity enemy;
        enemy.x = SCREEN_WIDTH;
        enemy.y = rand() % SCREEN_HEIGHT;
        enemy.texture = enemyTexture;
        SDL_QueryTexture(enemy.texture, nullptr, nullptr, &enemy.w, &enemy.h);
        enemy.dx = -(2 + (rand() % 4)); // [-2, -5]
        enemy.dy = rand() % 3 - 1;      // [-1, 1]
        enemy.health = 2;
        enemy.side = Side::Alien;
        enemy.reload = FPS * (1 + (rand() % 3));
        enemySpawnTimer = 60 + (rand() % 60);
        enemyList.emplace_back(move(enemy));
    }
}

void Stage::drawPlayer()
{
    if (!player)
        return;

    blit(renderer, player->texture, player->x, player->y);
}

void Stage::drawBullets()
{
    for (auto it = bulletList.begin(); it != bulletList.end(); it++)
    {
        blit(renderer, it->texture, it->x, it->y);
    }
}

void Stage::drawFighters()
{
    for (auto it = enemyList.begin(); it != enemyList.end(); it++)
    {
        blit(renderer, it->texture, it->x, it->y);
    }
}

void Stage::drawDebris()
{
    for (auto it = debrisList.begin(); it != debrisList.end(); it++)
    {
        blitRect(renderer, it->texture, &it->rect, it->x, it->y);
    }
}

void Stage::drawExplosions()
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

    for (auto it = explosionList.begin(); it != explosionList.end(); it++)
    {
        SDL_SetTextureColorMod(explosionTexture, it->r, it->g, it->b);
        SDL_SetTextureAlphaMod(explosionTexture, it->a);

        blit(renderer, explosionTexture, it->x, it->y);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Stage::drawHud()
{
    text->drawText(10, 10, 255, 255, 255, ALIGN_LEFT, "SCORE: %03d", score);

    if (score > 0 && score == highscore)
    {
        text->drawText(960, 10, 0, 255, 0, ALIGN_LEFT, "HIGH SCORE: %03d", highscore);
    }
    else
    {
        text->drawText(960, 10, 255, 255, 255, ALIGN_LEFT, "HIGH SCORE: %03d", highscore);
    }
}

void Stage::drawPointsPods()
{
    for (auto it = pointList.begin(); it != pointList.end(); it++)
    {
        blit(renderer, it->texture, it->x, it->y);
    }
}

// Stops the player from leave the screen and also prevents them from moving forward any further than about the midway point.
void Stage::clipPlayer()
{
    if (!player)
        return;

    if (player->x < 0)
    {
        player->x = 0;
    }
    if (player->y < 0)
    {
        player->y = 0;
    }
    if (player->x > SCREEN_WIDTH / 2)
    {
        player->x = SCREEN_WIDTH / 2;
    }
    if (player->y > SCREEN_HEIGHT - player->h)
    {
        player->y = SCREEN_HEIGHT - player->h;
    }
}