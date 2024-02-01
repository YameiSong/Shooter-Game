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
            break;

        case SDL_KEYUP:
            doKeyUp(&event.key);
            break;

        default:
            break;
        }
    }
}

void doKeyDown(SDL_KeyboardEvent *event)
{
    if (event->keysym.scancode == SDL_SCANCODE_UP)
    {
        app.up = true;
    }
    
    if (event->keysym.scancode == SDL_SCANCODE_DOWN)
    {
        app.down = true;
    }
    
    if (event->keysym.scancode == SDL_SCANCODE_LEFT)
    {
        app.left = true;
    }
    
    if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
    {
        app.right = true;
    }
    
    if (event->keysym.scancode == SDL_SCANCODE_LCTRL)
    {
        app.fire = true;
    }
}

void doKeyUp(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0)
    {
        if (event->keysym.scancode == SDL_SCANCODE_UP)
        {
            app.up = false;
        }
        
        if (event->keysym.scancode == SDL_SCANCODE_DOWN)
        {
            app.down = false;
        }
        
        if (event->keysym.scancode == SDL_SCANCODE_LEFT)
        {
            app.left = false;
        }
        
        if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
        {
            app.right = false;
        }
        
        if (event->keysym.scancode == SDL_SCANCODE_LCTRL)
        {
            app.fire = false;
        }
    }
}