#pragma once
#include "common.hpp"
#include "defs.hpp"
#include <functional>

struct Delegate
{
    std::function<void()> logic;
    std::function<void()> draw;
};

struct App
{
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    Delegate delegate;
    bool keyboard[MAX_KEYBOARD_KEYS];
};

struct Entity
{
    float x;
    float y;
    int w;
    int h;
    float dx;
    float dy;
    int health;
    int reload; // 8 frames (approx 0.133333 seconds) must pass before we can fire again
    SDL_Texture* texture;
};
