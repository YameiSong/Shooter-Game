#include "stage.hpp"
#include "common.hpp"
#include "structs.hpp"
#include "draw.hpp"

extern App app;

Stage::Stage()
{
    player.x = 100;
    player.y = 100;
    player.texture = loadTexture(PLAYER_TEXTURE_PATH);
    SDL_QueryTexture(player.texture, nullptr, nullptr, &player.w, &player.h);

    fighters.emplace_back(std::move(player));

    bulletTexture = loadTexture(BULLET_TEXTURE_PATH);
}

Stage::~Stage()
{
    SDL_DestroyTexture(player.texture);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Destory player texture");
    SDL_DestroyTexture(bulletTexture);
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Destory bullet texture");

    auto it_fighter = fighters.begin();
    while (it_fighter != fighters.end())
    {
        if (it_fighter->texture != nullptr)
        {
            SDL_DestroyTexture(it_fighter->texture);
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Destory fighter texture");
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
}

void Stage::logic()
{
    doPlayer();

    doBullets();
}

void Stage::draw()
{
    drawPlayer();

    drawBullets();
}

void Stage::doPlayer()
{
    player.dx = player.dy = 0;

    if (player.reload > 0)
    {
        player.reload--;
    }

    if (app.keyboard[SDL_SCANCODE_UP])
    {
        player.dy = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_DOWN])
    {
        player.dy = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_LEFT])
    {
        player.dx = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_RIGHT])
    {
        player.dx = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_LCTRL] && player.reload == 0)
    {
        fireBullet();
    }

    player.x += player.dx;
    player.y += player.dy;
}

void Stage::doBullets()
{
    auto it = bullets.begin();
    while (it != bullets.end())
    {
        it->x += it->dx;
        it->y += it->dy;

        if (it->x > SCREEN_WIDTH)
        {
            it = bullets.erase(it);
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

    bullet.x = player.x;
    bullet.y = player.y;
    bullet.dx = PLAYER_BULLET_SPEED;
    bullet.health = 1;
    bullet.texture = bulletTexture;
    SDL_QueryTexture(bullet.texture, nullptr, nullptr, &bullet.w, &bullet.h);

    bullet.y += (player.h / 2) - (bullet.h / 2);

    bullets.emplace_back(std::move(bullet));

    player.reload = 8;
}

void Stage::drawPlayer()
{
    blit(player.texture, player.x, player.y);
}

void Stage::drawBullets()
{
    auto it = bullets.begin();
    while (it != bullets.end())
    {
        blit(it->texture, it->x, it->y);
        it++;
    }
}
