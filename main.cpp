#include "init.hpp"
#include "input.hpp"
#include "draw.hpp"
#include "stage.hpp"

App app;
Stage stage;

int main(int argc, char const *argv[])
{
    u_int64_t then;
    float remainder;

    initSDL();

    atexit(cleanUp);

    Stage stage;

    app.delegate.logic = [&stage]()
    { stage.logic(); };

    app.delegate.draw = [&stage]()
    { stage.draw(); };

    then = SDL_GetTicks();

    remainder = 0;

    while (true)
    {
        prepareScene();

        doInput();

        app.delegate.logic();

        app.delegate.draw();

        presentScene();

        capFrameRate(then, remainder);
    }

    return 0;
}
