#include "input.hpp"
#include "structs.hpp"

extern App app;

void doInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;

        case SDL_KEYDOWN:
            doKeyDown(&event.key);

        case SDL_KEYUP:
            doKeyUp(&event.key);

        default:
            break;
        }
    }
}

void doKeyDown(SDL_KeyboardEvent *event)
{
    if (event->keysym.scancode == SDL_SCANCODE_UP)
    {
        app.up = 1;
    }
    else if (event->keysym.scancode == SDL_SCANCODE_DOWN)
    {
        app.down = 1;
    }
    else if (event->keysym.scancode == SDL_SCANCODE_LEFT)
    {
        app.left = 1;
    }
    else if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
    {
        app.right = 1;
    }
}

void doKeyUp(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0)
    {
        if (event->keysym.scancode == SDL_SCANCODE_UP)
        {
            app.up = 0;
        }
        else if (event->keysym.scancode == SDL_SCANCODE_DOWN)
        {
            app.down = 0;
        }
        else if (event->keysym.scancode == SDL_SCANCODE_LEFT)
        {
            app.left = 0;
        }
        else if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
        {
            app.right = 0;
        }
    }
}