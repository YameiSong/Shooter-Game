#include "Title.hpp"
#include <algorithm>

extern bool keyboard[MAX_KEYBOARD_KEYS];

Title::Title(SDL_Renderer *renderer,
             std::shared_ptr<Text> text) : renderer(renderer),
                                           text(text),
                                           timeout(FPS * 5),
                                           reveal(0)
{
    sdl2Texture = loadTexture(renderer, SDL2_TEXTURE_PATH);

    shooterTexture = loadTexture(renderer, SHOOTER_TEXTURE_PATH);
}

Title::~Title()
{
    SDL_DestroyTexture(sdl2Texture);

    SDL_DestroyTexture(shooterTexture);
}

void Title::draw()
{
    if (reveal < SCREEN_HEIGHT)
    {
        reveal++;
    }

    drawLogo();

    if (timeout % 40 < 20)
    {
        text->drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, FONT_COMMAND, ALIGN_CENTER, "%s", "PRESS FIRE TO PLAY!");
    }
}

void Title::drawLogo()
{
    SDL_Rect rect;

    rect.x = 0;
    rect.y = 0;

    SDL_QueryTexture(sdl2Texture, nullptr, nullptr, &rect.w, &rect.h);

    rect.h = std::min(reveal, rect.h);

    blitRect(renderer, sdl2Texture, &rect, SCREEN_WIDTH / 2 - rect.w / 2, 100);

    SDL_QueryTexture(shooterTexture, nullptr, nullptr, &rect.w, &rect.h);

    rect.h = std::min(reveal, rect.h);

    blitRect(renderer, shooterTexture, &rect, SCREEN_WIDTH / 2 - rect.w / 2, 250);
}