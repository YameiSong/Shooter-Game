#pragma once

#include "common.hpp"
#include "defs.hpp"
#include "draw.hpp"
#include <memory>

class Text
{
private:
    SDL_Texture *fontTexture = nullptr;
    std::unique_ptr<char[]> drawTextBuffer;
    SDL_Renderer *renderer;

public:
    Text(SDL_Renderer *renderer);
    ~Text();
    template <typename... Args>
    void drawText(int x, int y, int r, int g, int b, const char *format, Args... args);
};

template <typename... Args>
void Text::drawText(int x, int y, int r, int g, int b, const char *format, Args... args)
{
    int i, len, c;
    SDL_Rect rect;

    sprintf(drawTextBuffer.get(), format, args...);

    len = strlen(drawTextBuffer.get());

    rect.w = GLYPH_WIDTH;
    rect.h = GLYPH_HEIGHT;
    rect.y = 0;

    SDL_SetTextureColorMod(fontTexture, r, g, b);

    for (i = 0; i < len; i++)
    {
        c = drawTextBuffer[i];

        if (c >= ' ' && c <= 'Z')
        {
            rect.x = (c - ' ') * GLYPH_WIDTH;
            blitRect(renderer, fontTexture, &rect, x, y);
            x += GLYPH_WIDTH;
        }
    }
}