#include "init.hpp"
#include "draw.hpp"
#include "input.hpp"
#include "structs.hpp"
#include "defs.hpp"

App app;
Entity player;
Entity bullet;

int main(int argc, char const *argv[])
{
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));
    memset(&bullet, 0, sizeof(Entity));

    initSDL();

    atexit(cleanUp);

    player.x = 100;
    player.y = 100;
    player.texture = loadTexture("/home/zhifang/Documents/Shooter-Game/gfx/player.png");

    bullet.texture = loadTexture("/home/zhifang/Documents/Shooter-Game/gfx/playerBullet.png");
    int center_x, center_y;
    SDL_QueryTexture(player.texture, nullptr, nullptr, &center_x, &center_y);
    center_y /= 2;
    
    while (true)
    {
        prepareScene();

        doInput();

        player.x += player.dx;
        player.y += player.dy;

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

        if (app.fire && bullet.health == 0)
        {
            bullet.x = player.x + center_x;
            bullet.y = player.y + center_y;
            bullet.dx = 16;
            bullet.dy = 0;
            bullet.health = 1;
        }
        
        bullet.x += bullet.dx;
        bullet.y += bullet.dy;

        if (bullet.x > SCREEN_WIDTH)
        {
            bullet.health = 0;
        }
        
        blit(player.texture, player.x, player.y);
        
        if (bullet.health > 0)
        {
            blit(bullet.texture, bullet.x, bullet.y);
        }

        presentScene();

        SDL_Delay(16);
    }

    return 0;
}
