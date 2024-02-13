#pragma once

#include "common.hpp"
#include "defs.hpp"
#include "draw.hpp"
#include <memory>

class Text
{
private:
    SDL_Color white;
    std::unique_ptr<char[]> drawTextBuffer;
    SDL_Renderer *renderer;
    TTF_Font *fonts[FONT_MAX];
    SDL_Rect glyphs[FONT_MAX][NUME_GLYPHS];
    SDL_Texture *fontTextures[FONT_MAX];
    void initFont(int fontType, int fontSize, const char *filename);
    SDL_Texture *getTextTexture(char *text, int fontType);
    SDL_Texture *toTexture(SDL_Surface *surface, bool destroySurface);
    void calcTextDimensions(char* text, int fontType, int& w, int& h);
public:
    Text(SDL_Renderer *renderer);
    ~Text();
    template <typename... Args>
    void drawText(int x, int y, int r, int g, int b, int fontType, int align, const char *format, Args... args);
};

template <typename... Args>
void Text::drawText(int x, int y, int r, int g, int b, int fontType, int align, const char *format, Args... args)
{
    sprintf(drawTextBuffer.get(), format, args...);

    int w, h;

    if (align != ALIGN_LEFT)
    {
        calcTextDimensions(drawTextBuffer.get(), fontType, w, h);

        if (align == ALIGN_CENTER)
            x -= w / 2;
        else if (align == ALIGN_RIGHT)
            x -= w;
    }

    SDL_Rect *glyph, dest;

    if (SDL_SetTextureColorMod(fontTextures[fontType], r, g, b) < 0)
    {
        printf("SDL_SetTextureColorMod failed: %s\n", SDL_GetError());
        exit(1);
    }

    int ch;

    for (int i = 0; drawTextBuffer[i] != '\0'; i++)
    {
        ch = drawTextBuffer[i];
        glyph = &glyphs[fontType][ch];

        dest.x = x;
        dest.y = y;
        dest.w = glyph->w;
        dest.h = glyph->h;

        if (SDL_RenderCopy(renderer, fontTextures[fontType], glyph, &dest) < 0)
        {
            printf("SDL_RenderCopy failed: %s\n", SDL_GetError());
            exit(1);
        }

        x += glyph->w;
    }
}