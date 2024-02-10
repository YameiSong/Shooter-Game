#include "draw.hpp"
#include "App.hpp"

bool keyboard[MAX_KEYBOARD_KEYS];

int main()
{
    u_int64_t then;
    float remainder;

    App app;

    then = SDL_GetTicks();

    remainder = 0;

    while (true)
    {
        prepareScene(app.renderer);

        app.doInput();

        app.logic();

        app.draw();

        presentScene(app.renderer);

        capFrameRate(then, remainder);
    }

    return 0;
}
