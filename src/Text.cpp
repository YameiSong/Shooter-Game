#include "Text.hpp"

Text::Text(SDL_Renderer *renderer) : drawTextBuffer(new char[MAX_LINE_LENGTH]), renderer(renderer)
{
    fontTexture = loadTexture(renderer, FONT_TEXTURE_PATH);
}

Text::~Text()
{
    if (fontTexture != nullptr)
        SDL_DestroyTexture(fontTexture);
}