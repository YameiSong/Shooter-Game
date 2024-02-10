#pragma once

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FPS 60
#define MAX_KEYBOARD_KEYS 350
#define MAX_LINE_LENGTH 1024
#define MAX_SCORE_NAME_LENGTH 16
#define GLYPH_WIDTH 18
#define GLYPH_HEIGHT 24

#define PLAYER_SPEED 4
#define PLAYER_BULLET_SPEED 16
#define ALIEN_BULLET_SPEED 8
#define MAX_STARS 500
#define NUM_HIGHSCORES 8
#define ANONYMOUS "ANONYMOUS"

#define PLAYER_TEXTURE_PATH "gfx/player.png"
#define BULLET_TEXTURE_PATH "gfx/playerBullet.png"
#define ENEMY_TEXTURE_PATH "gfx/enemy.png"
#define ALIEN_BULLET_TEXTURE_PATH "gfx/alienBullet.png"
#define BACKGROUND_TEXTURE_PATH "gfx/background.png"
#define EXPLOSION_TEXTURE_PATH "gfx/explosion.png"
#define POINTS_TEXTURE_PATH "gfx/points.png"

#define FONT_TEXTURE_PATH "gfx/font.png"

#define MAX_SOUND_CHANNELS 8

#define PLAYER_FIRE_SOUND_PATH "sound/334227__jradcoolness__laser.ogg"
#define ALIEN_FIRE_SOUND_PATH "sound/196914__dpoggioli__laser-gun.ogg"
#define PLAYER_DIE_SOUND_PATH "sound/245372__quaker540__hq-explosion.ogg"
#define ALIEN_DIE_SOUND_PATH "sound/10 Guage Shotgun-SoundBible.com-74120584.ogg"
#define POINTS_SOUND_PATH "sound/342749__rhodesmas__notification-01.ogg"
#define BGM "music/Mercury.ogg"

enum Channel {
    CH_Any = -1,
    CH_Player,
    CH_Alien_Fire,
    CH_POINTS
};

enum Sound {
    SND_Player_Fire,
    SND_Alien_Fire,
    SND_Player_Die,
    SND_Alien_Die,
    SND_POINTS,
    SND_Max
};

enum TEXT_ALIGN {
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
};