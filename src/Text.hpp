#pragma once

#include "common.hpp"
#include "defs.hpp"
#include "draw.hpp"
#include <memory>

class Text
{
private:
    SDL_Texture *fontTexture = nullptr;
    SDL_Color white;
    std::unique_ptr<char[]> drawTextBuffer;
    SDL_Renderer *renderer;
    TTF_Font *titleFont;
    TTF_Font *cmdFont;
    SDL_Texture *getTextTexture(char *text, bool isTitle);
    SDL_Texture *toTexture(SDL_Surface *surface, bool destroySurface);

public:
    Text(SDL_Renderer *renderer);
    ~Text();
    template <typename... Args>
    void drawText(int x, int y, int r, int g, int b, int align, bool isTitle, const char *format, Args... args);
};

template <typename... Args>
void Text::drawText(int x, int y, int r, int g, int b, int align, bool isTitle, const char *format, Args... args)
{
    sprintf(drawTextBuffer.get(), format, args...);

    int len = strlen(drawTextBuffer.get());

    // TODO: align

    SDL_Texture *texture = getTextTexture(drawTextBuffer.get(), isTitle, color);

    SDL_SetTextureColorMod(texture, r, g, b);

    blit(renderer, texture, x, y);
}