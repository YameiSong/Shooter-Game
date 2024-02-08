#include "audio_player.hpp"

AudioPlayer::AudioPlayer()
{
    for (auto &&snd : sounds)
    {
        snd = nullptr;
    }

    loadSounds();
}

AudioPlayer::~AudioPlayer()
{
    for (auto &&snd : sounds)
    {
        if (snd != nullptr)
        {
            Mix_FreeChunk(snd);
        }
    }

    if (music != nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = nullptr;
    }
}

void AudioPlayer::loadSounds()
{
    sounds[SND_Player_Fire] = Mix_LoadWAV(PLAYER_FIRE_SOUND_PATH);
    sounds[SND_Alien_Fire] = Mix_LoadWAV(ALIEN_FIRE_SOUND_PATH);
    sounds[SND_Player_Die] = Mix_LoadWAV(PLAYER_DIE_SOUND_PATH);
    sounds[SND_Alien_Die] = Mix_LoadWAV(ALIEN_DIE_SOUND_PATH);
    sounds[SND_POINTS] = Mix_LoadWAV(POINTS_SOUND_PATH);
}

void AudioPlayer::loadMusic(std::string filename)
{
    if (music != nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = nullptr;
    }

    music = Mix_LoadMUS(filename.c_str());
}

void AudioPlayer::playMusic(int loop)
{
    Mix_PlayMusic(music, loop ? -1 : 0);
}

void AudioPlayer::playSound(int id, int channel)
{
    Mix_PlayChannel(channel, sounds[id], 0);
}