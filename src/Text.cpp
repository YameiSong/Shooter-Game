#include "Text.hpp"

Text::Text(SDL_Renderer *renderer) : drawTextBuffer(new char[MAX_LINE_LENGTH]),
                                     renderer(renderer)
{
    white = {0, 0, 0, 0};
    initFont(FONT_COMMAND, 16, FONT_PATH);
    initFont(FONT_TITLE, 32, FONT_PATH);
}

Text::~Text()
{
}

void Text::initFont(int fontType, int fontSize, const char *filename)
{
    SDL_Surface *surface, *text;
    SDL_Rect dest;
    char c[2];
    SDL_Rect *g;

    memset(&glyphs[fontType], 0, sizeof(SDL_Rect) * NUME_GLYPHS);

    fonts[fontType] = TTF_OpenFont(filename, fontSize);

    // treat white as transparent
    surface = SDL_CreateRGBSurface(0, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, 32, 0, 0, 0, 0xff);
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

    dest.x = dest.y = 0;

    for (int i = ' '; i <= 'z'; i++)
    {
        c[0] = i;
        c[1] = '\0';

        // initially render text in white, so that it can be easily blended with other colors later
        text = TTF_RenderUTF8_Blended(fonts[fontType], c, white);

        TTF_SizeText(fonts[fontType], c, &dest.w, &dest.h);

        // current line is filled with chars, blit char to the next line on the surface
        if (dest.x + dest.w >= FONT_TEXTURE_SIZE)
        {
            dest.x = 0;
            dest.y += dest.h + 1;
            if (dest.y + dest.h >= FONT_TEXTURE_SIZE)
            {
                SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "Out of glyph space in %dx%d font atlas texture map.", FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE);
                exit(1);
            }
        }

        SDL_BlitSurface(text, nullptr, surface, &dest);

        g = &glyphs[fontType][i];

        g->x = dest.x;
        g->y = dest.y;
        g->w = dest.w;
        g->h = dest.h;

        SDL_FreeSurface(text);

        dest.x += dest.w;
    }

    fontTextures[fontType] = toTexture(surface, true);
}

SDL_Texture *Text::getTextTexture(char *text, int fontType)
{
    SDL_Surface *surface;

    surface = TTF_RenderUTF8_Blended(fonts[fontType], text, white);

    return toTexture(surface, true);
}

SDL_Texture *Text::toTexture(SDL_Surface *surface, bool destroySurface)
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (destroySurface)
    {
        SDL_FreeSurface(surface);
    }

    return texture;
}
