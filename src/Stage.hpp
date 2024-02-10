#pragma once

#include "Text.hpp"
#include "AudioPlayer.hpp"
#include <list>
#include <memory>

class Stage
{
    enum class Side;
    struct Explosion;
    struct Debris;
    struct Entity;
    using EntityIt = std::list<Entity>::iterator;

private:
    void initPlayer();
    void doPlayer();
    void doBullets();
    void doEnemies();
    void doExplosions();
    void doDebris();
    void doPointsPods();
    void spawnEnemies();
    void fireBullet();
    void fireAlienBullet(EntityIt enemy);
    bool bulletHitEnemy(EntityIt bullet);
    bool bulletHitPlayer(EntityIt bullet);
    bool enemyHitPlayer(EntityIt enemy);
    void addExplosions(int x, int y, int num);
    void addDebris(Entity *e);
    void addPointsPod(int x, int y);
    void drawPlayer();
    void drawBullets();
    void drawFighters();
    void drawDebris();
    void drawExplosions();
    void drawHud();
    void drawPointsPods();
    void clipPlayer();

    int highscore;
    int enemySpawnTimer = 0;
    int stageResetTimer = FPS * 2;

    std::unique_ptr<Entity> player;
    SDL_Texture *playerTexture;
    SDL_Texture *bulletTexture;
    SDL_Texture *enemyTexture;
    SDL_Texture *pointsTexture;
    SDL_Texture *alienBulletTexture;
    SDL_Texture *explosionTexture;
    std::list<Entity> enemyList;
    std::list<Entity> bulletList;
    std::list<Entity> pointList;
    std::list<Explosion> explosionList;
    std::list<Debris> debrisList;
    SDL_Renderer *renderer;
    std::shared_ptr<AudioPlayer> audio_player;
    std::shared_ptr<Text> text;

public:
    Stage(int highscore, SDL_Renderer *renderer, std::shared_ptr<AudioPlayer> audio_player, std::shared_ptr<Text> text);
    ~Stage();
    void resetStage();
    void logic();
    void draw();
    bool gameover = false;
    int score = 0;
};
