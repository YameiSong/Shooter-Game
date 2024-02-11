#include "Text.hpp"

Text::Text(SDL_Renderer *renderer) : drawTextBuffer(new char[MAX_LINE_LENGTH]),
                                     renderer(renderer)
{
    white = {0, 0, 0, 0};
    titleFont = TTF_OpenFont(FONT_PATH, 32);
    cmdFont = TTF_OpenFont(FONT_PATH, 16);
}

Text::~Text()
{
}

SDL_Texture *Text::getTextTexture(char *text, bool isTitle)
{
    SDL_Surface *surface;

    if (isTitle)
    {
        surface = TTF_RenderUTF8_Blended(titleFont, text, white);
    }
    else
    {
        surface = TTF_RenderUTF8_Blended(cmdFont, text, white);
    }

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
