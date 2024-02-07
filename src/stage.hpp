#include "structs.hpp"
#include "audio_player.hpp"
#include <list>
#include <memory>
#include <vector>

class Stage
{
    using EntityIt = std::list<Entity>::iterator;

private:
    void resetStage();
    void initPlayer();
    void initStarfield();
    void doPlayer();
    void doBullets();
    void doEnemies();
    void doBackground();
    void doStarfield();
    void doExplosions();
    void doDebris();
    void spawnEnemies();
    void fireBullet();
    void fireAlienBullet(EntityIt enemy);
    bool bulletHitEnemy(EntityIt bullet);
    bool bulletHitPlayer(EntityIt bullet);
    bool enemyHitPlayer(EntityIt enemy);
    void addExplosions(int x, int y, int num);
    void addDebris(Entity* e);
    void drawPlayer();
    void drawBullets();
    void drawFighters();
    void drawBackground();
    void drawStarfield();
    void drawDebris();
    void drawExplosions();
    void clipPlayer();

    int enemySpawnTimer = 0;
    int stageResetTimer = FPS * 2;
    int backgroundX = 0;
    std::unique_ptr<Entity> player;
    SDL_Texture *playerTexture;
    SDL_Texture *bulletTexture;
    SDL_Texture *enemyTexture;
    SDL_Texture *alienBulletTexture;
    SDL_Texture *backgroundTexture;
    SDL_Texture *explosionTexture;
    std::list<Entity> list_enemy;
    std::list<Entity> list_bullet;
    std::list<Explosion> list_explosion;
    std::list<Debris> list_debris;
    std::vector<Star> stars;
    AudioPlayer audio_player;

public:
    Stage();
    ~Stage();
    void logic();
    void draw();
};
