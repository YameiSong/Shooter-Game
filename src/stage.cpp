#include "stage.hpp"
#include "common.hpp"
#include "structs.hpp"
#include "draw.hpp"
#include "util.hpp"
#include <cstdlib>
#include <ctime>

extern App app;

Stage::Stage() : player(nullptr)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    playerTexture = loadTexture(PLAYER_TEXTURE_PATH);

    bulletTexture = loadTexture(BULLET_TEXTURE_PATH);

    enemyTexture = loadTexture(ENEMY_TEXTURE_PATH);

    alienBulletTexture = loadTexture(ALIEN_BULLET_TEXTURE_PATH);

    initPlayer();
}

Stage::~Stage()
{
    SDL_DestroyTexture(player->texture);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Destory player texture");
    SDL_DestroyTexture(bulletTexture);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Destory bullet texture");

    auto it_fighter = enemies.begin();
    while (it_fighter != enemies.end())
    {
        if (it_fighter->texture != nullptr)
        {
            SDL_DestroyTexture(it_fighter->texture);
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Destory enemy texture");
        }
        it_fighter++;
    }

    auto it_bullet = bullets.begin();
    while (it_bullet != bullets.end())
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
    doPlayer();

    doEnemies();

    doBullets();

    spawnEnemies();

    clipPlayer();

    if (player.get() == nullptr && --stageResetTimer <= 0)
    {
        resetStage();
    }
}

void Stage::draw()
{
    drawPlayer();

    drawBullets();

    drawFighters();
}

void Stage::initPlayer()
{
    player.reset(new Entity);
    player->x = 100;
    player->y = 100;
    player->side = Side::Player;
    player->health = 10;
    player->texture = playerTexture;
    SDL_QueryTexture(player->texture, nullptr, nullptr, &player->w, &player->h);
}

void Stage::resetStage()
{
    enemies.clear();
    bullets.clear();

    initPlayer();

    enemySpawnTimer = 0;

    stageResetTimer = FPS * 2;
}

void Stage::doPlayer()
{
    if (player.get() == nullptr)
    {
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

    if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0)
    {
        fireBullet();
    }

    player->x += player->dx;
    player->y += player->dy;
}

void Stage::doBullets()
{
    auto it = bullets.begin();
    while (it != bullets.end())
    {
        it->x += it->dx;
        it->y += it->dy;

        if (it->x > SCREEN_WIDTH || bulletHitFighter(it))
        {
            it = bullets.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Stage::doEnemies()
{
    auto it = enemies.begin();
    while (it != enemies.end())
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
            it = enemies.erase(it);
        }
        else
        {
            if (player.get() != nullptr && --it->reload <= 0)
            {
                fireAlienBullet(it);
            }
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

    bullets.emplace_back(std::move(bullet));

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

    enemy->reload = (std::rand() % FPS * 2);
}

bool Stage::bulletHitFighter(EntityIt bullet)
{
    for (auto it = enemies.begin(); it != enemies.end(); it++)
    {
        if (bullet->side != it->side &&
            collision(bullet->x, bullet->y, bullet->w, bullet->h, it->x, it->y, it->w, it->h))
        {
            bullet->health = 0;
            it->health--;
            return true;
        }
    }
    return false;
}

bool Stage::enemyHitPlayer(EntityIt enemy)
{
    if (collision(player->x, player->y, player->w, player->h, enemy->x, enemy->y, enemy->w, enemy->h))
    {
        player->health--;
        enemy->health = 0;
        return true;
    }
    return false;
}

void Stage::spawnEnemies()
{
    if (--enemySpawnTimer <= 0)
    {
        Entity enemy;
        enemy.x = SCREEN_WIDTH;
        enemy.y = std::rand() % SCREEN_HEIGHT;
        enemy.texture = enemyTexture;
        SDL_QueryTexture(enemy.texture, nullptr, nullptr, &enemy.w, &enemy.h);
        enemy.dx = -(2 + (std::rand() % 4)); // [-2, -5]
        enemy.dy = std::rand() % 3 - 1;      // [-1, 1]
        enemy.health = 2;
        enemy.side = Side::Alien;
        enemySpawnTimer = 60 + (std::rand() % 60);
        enemies.emplace_back(std::move(enemy));
    }
}

void Stage::drawPlayer()
{
    blit(player->texture, player->x, player->y);
}

void Stage::drawBullets()
{
    for (auto it = bullets.begin(); it != bullets.end(); it++)
    {
        blit(it->texture, it->x, it->y);
    }
}

void Stage::drawFighters()
{
    for (auto it = enemies.begin(); it != enemies.end(); it++)
    {
        blit(it->texture, it->x, it->y);
    }
}