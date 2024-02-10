#include "Stage.hpp"
#include "HighscoreTable.hpp"
#include "Text.hpp"
#include "AudioPlayer.hpp"
#include "Background.hpp"
#include <memory>

class App
{
public:
    App();
    ~App();
    void initSDL();
    void cleanUp();
    void logic();
    void draw();
    void doInput();
    void doKeyDown(SDL_KeyboardEvent *event);
    void doKeyUp(SDL_KeyboardEvent *event);
    bool playing = false;
    SDL_Renderer *renderer;
    SDL_Window *window;
    std::unique_ptr<Background> background;
    std::shared_ptr<AudioPlayer> audio_player;
    std::shared_ptr<Text> text;
    std::unique_ptr<Stage> stage;
    std::unique_ptr<HighscoreTable> highscore_table;
};