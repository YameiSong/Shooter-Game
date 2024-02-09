#include "Text.hpp"

Text::Text() : drawTextBuffer(new char[MAX_LINE_LENGTH])
{
    fontTexture = loadTexture(FONT_TEXTURE_PATH);
}

Text::~Text()
{
    if (fontTexture != nullptr)
        SDL_DestroyTexture(fontTexture);
}