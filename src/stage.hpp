#include "structs.hpp"
#include <list>
#include <memory>

class Stage
{
    using EntityIt = std::list<Entity>::iterator;

private:
    void resetStage();
    void initPlayer();
    void doPlayer();
    void doBullets();
    void doEnemies();
    void spawnEnemies();
    void fireBullet();
    void fireAlienBullet(EntityIt enemy);
    bool bulletHitFighter(EntityIt bullet);
    bool enemyHitPlayer(EntityIt enemy);
    void drawPlayer();
    void drawBullets();
    void drawFighters();
    void clipPlayer();

    int enemySpawnTimer = 0;
    int stageResetTimer = FPS * 2;
    std::unique_ptr<Entity> player;
    SDL_Texture *playerTexture;
    SDL_Texture *bulletTexture;
    SDL_Texture *enemyTexture;
    SDL_Texture *alienBulletTexture;
    std::list<Entity> enemies;
    std::list<Entity> bullets;

public:
    Stage();
    ~Stage();
    void logic();
    void draw();
};
