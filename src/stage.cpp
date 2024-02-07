#include "stage.hpp"
#include "common.hpp"
#include "structs.hpp"
#include "draw.hpp"
#include "util.hpp"
#include <cstdlib>
#include <ctime>

using namespace std;

extern App app;

Stage::Stage() : player(nullptr), stars(MAX_STARS), audio_player()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    playerTexture = loadTexture(PLAYER_TEXTURE_PATH);

    bulletTexture = loadTexture(BULLET_TEXTURE_PATH);

    enemyTexture = loadTexture(ENEMY_TEXTURE_PATH);

    alienBulletTexture = loadTexture(ALIEN_BULLET_TEXTURE_PATH);

    backgroundTexture = loadTexture(BACKGROUND_TEXTURE_PATH);

    explosionTexture = loadTexture(EXPLOSION_TEXTURE_PATH);

    resetStage();
}

Stage::~Stage()
{
    if (player.get() != nullptr)
    {
        SDL_DestroyTexture(player->texture);
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Destory player texture");
    }
    SDL_DestroyTexture(bulletTexture);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Destory bullet texture");

    auto it_fighter = list_enemy.begin();
    while (it_fighter != list_enemy.end())
    {
        if (it_fighter->texture != nullptr)
        {
            SDL_DestroyTexture(it_fighter->texture);
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Destory enemy texture");
        }
        it_fighter++;
    }

    auto it_bullet = list_bullet.begin();
    while (it_bullet != list_bullet.end())
    {
        if (it_bullet->texture != nullptr)
        {
            SDL_DestroyTexture(it_bullet->texture);
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Destory bullet texture");
        }
        it_bullet++;
    }

    SDL_DestroyTexture(enemyTexture);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Destory enemy texture");

    SDL_DestroyTexture(alienBulletTexture);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Destory alien bullet texture");
}

void Stage::logic()
{
    doBackground();

    doStarfield();

    doPlayer();

    doEnemies();

    doBullets();

    spawnEnemies();

    doExplosions();

    doDebris();

    clipPlayer();

    if (player.get() == nullptr && --stageResetTimer <= 0)
    {
        resetStage();
    }
}

void Stage::draw()
{
    drawBackground();

    drawStarfield();

    drawPlayer();

    drawBullets();

    drawFighters();

    drawDebris();

    drawExplosions();
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

void Stage::initStarfield()
{
    for (size_t i = 0; i < MAX_STARS; i++)
    {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % SCREEN_HEIGHT;
        stars[i].speed = 1 + rand() % 8;
    }
}

void Stage::resetStage()
{
    list_enemy.clear();
    list_bullet.clear();
    list_explosion.clear();
    list_debris.clear();

    initPlayer();

    initStarfield();

    enemySpawnTimer = 0;

    stageResetTimer = FPS * 3;
}

void Stage::doPlayer()
{
    if (player.get() == nullptr)
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

    if (app.keyboard[SDL_SCANCODE_UP])
    {
        player->dy = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_DOWN])
    {
        player->dy = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_LEFT])
    {
        player->dx = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_RIGHT])
    {
        player->dx = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload <= 0)
    {
        fireBullet();
        audio_player.playSound(SND_Player_Fire, CH_Player);
    }

    player->x += player->dx;
    player->y += player->dy;
}

void Stage::doBullets()
{
    auto it = list_bullet.begin();
    while (it != list_bullet.end())
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
            it = list_bullet.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Stage::doEnemies()
{
    auto it = list_enemy.begin();
    while (it != list_enemy.end())
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
            it = list_enemy.erase(it);
        }
        else
        {
            if (player.get() != nullptr && --it->reload <= 0)
            {
                fireAlienBullet(it);
                audio_player.playSound(SND_Alien_Fire, CH_Alien_Fire);
            }
            it++;
        }
    }
}

void Stage::doBackground()
{
    if (--backgroundX < -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
}

void Stage::doStarfield()
{
    for (size_t i = 0; i < MAX_STARS; i++)
    {
        stars[i].x -= stars[i].speed;

        if (stars[i].x < 0)
        {
            stars[i].x = SCREEN_WIDTH + stars[i].x;
        }
    }
}

void Stage::doExplosions()
{
    auto it = list_explosion.begin();
    while (it != list_explosion.end())
    {
        it->x += it->dx;
        it->y += it->dy;

        if (--it->a <= 0)
        {
            it = list_explosion.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Stage::doDebris()
{
    auto it = list_debris.begin();
    while (it != list_debris.end())
    {
        it->x += it->dx;
        it->y += it->dy;
        it->dy += 0.5;

        if (--it->life <= 0)
        {
            it = list_debris.erase(it);
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

    list_bullet.emplace_back(move(bullet));

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

    list_bullet.emplace_back(move(bullet));

    enemy->reload = (rand() % FPS * 2);
}

bool Stage::bulletHitEnemy(EntityIt bullet)
{
    for (auto it = list_enemy.begin(); it != list_enemy.end(); it++)
    {
        if (bullet->side != it->side &&
            collision(bullet->x, bullet->y, bullet->w, bullet->h, it->x, it->y, it->w, it->h))
        {
            audio_player.playSound(SND_Alien_Die, CH_Any); // play multiple explosion sounds at the same time
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
    if (player.get() == nullptr)
        return false;

    if (bullet->side != player->side &&
        collision(bullet->x, bullet->y, bullet->w, bullet->h, player->x, player->y, player->w, player->h))
    {
        audio_player.playSound(SND_Player_Die, CH_Player);
        bullet->health = 0;
        player->health--;
        return true;
    }
    return false;
}

bool Stage::enemyHitPlayer(EntityIt enemy)
{
    if (player.get() == nullptr)
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

        list_explosion.emplace_back(move(e));
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

            list_debris.emplace_back(move(d));
        }
    }
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
        list_enemy.emplace_back(move(enemy));
    }
}

void Stage::drawPlayer()
{
    if (player.get() == nullptr)
        return;

    blit(player->texture, player->x, player->y);
}

void Stage::drawBullets()
{
    for (auto it = list_bullet.begin(); it != list_bullet.end(); it++)
    {
        blit(it->texture, it->x, it->y);
    }
}

void Stage::drawFighters()
{
    for (auto it = list_enemy.begin(); it != list_enemy.end(); it++)
    {
        blit(it->texture, it->x, it->y);
    }
}

void Stage::drawBackground()
{
    SDL_Rect dest;

    for (int x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
    {
        dest.x = x;
        dest.y = 0;
        dest.w = SCREEN_WIDTH;
        dest.h = SCREEN_HEIGHT;

        SDL_RenderCopy(app.renderer, backgroundTexture, nullptr, &dest);
    }
}

void Stage::drawStarfield()
{
    for (size_t i = 0; i < MAX_STARS; i++)
    {
        int c = 32 * stars[i].speed;

        SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

        SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
    }
}

void Stage::drawDebris()
{
    for (auto it = list_debris.begin(); it != list_debris.end(); it++)
    {
        blitRect(it->texture, &it->rect, it->x, it->y);
    }
}

void Stage::drawExplosions()
{
    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

    for (auto it = list_explosion.begin(); it != list_explosion.end(); it++)
    {
        SDL_SetTextureColorMod(explosionTexture, it->r, it->g, it->b);
        SDL_SetTextureAlphaMod(explosionTexture, it->a);

        blit(explosionTexture, it->x, it->y);
    }

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}

// Stops the player from leave the screen and also prevents them from moving forward any further than about the midway point.
void Stage::clipPlayer()
{
    if (player.get() == nullptr)
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