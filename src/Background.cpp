#include "Background.hpp"
#include "defs.hpp"
#include "draw.hpp"
#include "App.hpp"

extern App app;

Background::Background(
    SDL_Renderer *renderer) : stars(MAX_STARS),
                                              renderer(renderer),
                                              backgroundTexture(nullptr)
{
    backgroundTexture = loadTexture(renderer, BACKGROUND_TEXTURE_PATH);
    initStarfield();
}

Background::~Background()
{
    SDL_DestroyTexture(backgroundTexture);
    backgroundTexture = nullptr;
}

void Background::initStarfield()
{
    for (size_t i = 0; i < MAX_STARS; i++)
    {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % SCREEN_HEIGHT;
        stars[i].speed = 1 + rand() % 8;
    }
}

void Background::doBackground()
{
    if (--backgroundX < -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
}

void Background::doStarfield()
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

void Background::drawBackground()
{
    SDL_Rect dest;

    for (int x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
    {
        dest.x = x;
        dest.y = 0;
        dest.w = SCREEN_WIDTH;
        dest.h = SCREEN_HEIGHT;

        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &dest);
    }
}

void Background::drawStarfield()
{
    for (size_t i = 0; i < MAX_STARS; i++)
    {
        int c = 32 * stars[i].speed;

        SDL_SetRenderDrawColor(renderer, c, c, c, 255);

        SDL_RenderDrawLine(renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
    }
}
