#include "draw.hpp"
#include "structs.hpp"

extern App app;

void prepareScene()
{
    SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene()
{
    SDL_RenderPresent(app.renderer);
}

SDL_Texture *loadTexture(std::string filename)
{
    SDL_Texture *texture;
    texture = IMG_LoadTexture(app.renderer, filename.c_str());
    return texture;
}

void blit(SDL_Texture *texture, int x, int y)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);
    SDL_RenderCopy(app.renderer, texture, nullptr, &dest);
}

void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = src->w;
    dest.h = src->h;

    SDL_RenderCopy(app.renderer, texture, src, &dest);
}

// Adjust frame rate to 60fps.
void capFrameRate(uint64_t &then, float &remainder)
{
    uint64_t wait, frameTime;

    wait = 16 + remainder;

    remainder -= static_cast<int>(remainder);

    frameTime = SDL_GetTicks64() - then;

    wait -= frameTime;

    if (wait < 1)
        wait = 1;

    SDL_Delay(wait);

    remainder += 0.667;

    then = SDL_GetTicks64();
}