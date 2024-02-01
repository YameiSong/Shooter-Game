#pragma once
#include "common.hpp"

struct App
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    int up;
    int down;
    int left;
    int right;
};

struct Entity
{
    int x;
    int y;
    SDL_Texture *texture;
};
