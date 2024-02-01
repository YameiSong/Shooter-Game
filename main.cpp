#include "init.hpp"
#include "draw.hpp"
#include "input.hpp"
#include "structs.hpp"

App app;
Entity player;

int main(int argc, char const *argv[])
{
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));

    initSDL();

    player.x = 100;
    player.y = 100;
    player.texture = loadTexture("/home/zhifang/Documents/shooter-0.9.0/gfx/player.png");

    atexit(cleanUp);

    while (true)
    {
        prepareScene();

        doInput();

        if (app.up)
        {
            player.y -= 4;
        }
        if (app.down)
        {
            player.y += 4;
        }
        if (app.left)
        {
            player.x -= 4;
        }
        if (app.right)
        {
            player.x += 4;
        }

        blit(player.texture, player.x, player.y);

        presentScene();

        SDL_Delay(16);
    }

    return 0;
}
