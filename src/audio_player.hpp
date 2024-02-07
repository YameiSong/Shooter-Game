#include "common.hpp"
#include "defs.hpp"
#include <array>
#include <string>

class AudioPlayer
{
private:
    Mix_Music* music = nullptr;
    std::array<Mix_Chunk*, SND_Max> sounds;
public:
    AudioPlayer();
    ~AudioPlayer();
    void loadSounds();
    void loadMusic(std::string filename);
    void playMusic(int loop);
    void playSound(int id, int channel);
};