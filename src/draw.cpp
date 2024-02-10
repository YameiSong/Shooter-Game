#include "draw.hpp"
#include "structs.hpp"

void prepareScene(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 96, 128, 255, 255);
    SDL_RenderClear(renderer);
}

void presentScene(SDL_Renderer* renderer)
{
    SDL_RenderPresent(renderer);
}

SDL_Texture *loadTexture(SDL_Renderer* renderer, std::string filename)
{
    SDL_Texture *texture;
    texture = IMG_LoadTexture(renderer, filename.c_str());
    return texture;
}

void blit(SDL_Renderer* renderer, SDL_Texture *texture, int x, int y)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, nullptr, &dest);
}

void blitRect(SDL_Renderer* renderer, SDL_Texture *texture, SDL_Rect *src, int x, int y)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = src->w;
    dest.h = src->h;

    SDL_RenderCopy(renderer, texture, src, &dest);
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