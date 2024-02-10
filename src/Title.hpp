#pragma once

#include "Text.hpp"

class Title
{
private:
    int reveal;
    SDL_Renderer *renderer;
    SDL_Texture *sdl2Texture;
    SDL_Texture *shooterTexture;
    std::shared_ptr<Text> text;

public:
    Title(SDL_Renderer *renderer, std::shared_ptr<Text> text);
    ~Title();
    void draw();
    void drawLogo();
    int timeout;
};