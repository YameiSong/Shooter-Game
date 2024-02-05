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

enum class Side
{
    Player,
    Alien
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
    Side side;
    SDL_Texture *texture;
};

struct Explosion
{
    float x;
    float y;
    float dx;
    float dy;
    int r, g, b, a;
};

struct Debris
{
    float x;
    float y;
    float dx;
    float dy;
    int life;
    SDL_Rect rect;
    SDL_Texture *texture;
};

struct Star
{
    int x;
    int y;
    int speed;
};
