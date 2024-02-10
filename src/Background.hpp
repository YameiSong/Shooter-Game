#pragma once

#include "common.hpp"
#include <vector>
#include <memory>

class Background
{
private:
    struct Star
    {
        int x;
        int y;
        int speed;
    };
    int backgroundX = 0;
    std::vector<Star> stars;
    SDL_Texture* backgroundTexture;
    SDL_Renderer *renderer;

public:
    Background(SDL_Renderer *renderer);
    ~Background();
    void initStarfield();
    void doBackground();
    void doStarfield();
    void drawBackground();
    void drawStarfield();
};