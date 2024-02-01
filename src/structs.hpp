#pragma once
#include "common.hpp"

struct App
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    bool up;
    bool down;
    bool left;
    bool right;
    bool fire;
};

struct Entity
{
    int x;
    int y;
    int dx;
    int dy;
    int health;
    SDL_Texture *texture;
};
