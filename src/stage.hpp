#include "structs.hpp"
#include <list>

class Stage
{
private:
    void doPlayer();
    void doBullets();
    void fireBullet();
    void drawPlayer();
    void drawBullets();

    Entity player;
    SDL_Texture* bulletTexture;
    std::list<Entity> fighters;
    std::list<Entity> bullets;

public:
    Stage();
    ~Stage();
    void logic();
    void draw();
};
