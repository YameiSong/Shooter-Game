#include "App.hpp"

extern bool keyboard[MAX_KEYBOARD_KEYS];

App::App() : renderer(nullptr),
             window(nullptr),
             background(nullptr),
             text(nullptr),
             audio_player(nullptr),
             stage(nullptr),
             highscore_table(nullptr)
{
    initSDL();

    background.reset(new Background(renderer));

    text.reset(new Text(renderer));

    audio_player.reset(new AudioPlayer);

    stage.reset(new Stage(0, renderer, audio_player, text));

    highscore_table.reset(new HighscoreTable(renderer, text));

    audio_player->loadMusic(BGM);
    audio_player->playMusic(1);
}

App::~App()
{
    cleanUp();
}

void App::initSDL()
{
    int rendererFlags, windowFlags;
    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("Shooter",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              windowFlags);

    if (!window)
    {
        printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    renderer = SDL_CreateRenderer(window, -1, rendererFlags);

    if (!renderer)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    // SDL_ShowCursor(SDL_DISABLE);

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
    {
        printf("Couldn't initialize SDL Mixer\n");
        SDL_Log("Mixer Initialization Error: %s", Mix_GetError());
        exit(1);
    }

    Mix_AllocateChannels(MAX_SOUND_CHANNELS);
}

void App::cleanUp()
{
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Destory renderer");

    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Destory window");

    SDL_Quit();
}

void App::logic()
{
    background->doBackground();
    background->doStarfield();

    if (!playing)
    {
        highscore_table->logic(inputText);

        if (keyboard[SDL_SCANCODE_LCTRL])
        {
            playing = true;
        }
    }

    if (playing)
    {
        if (stage->gameover)
        {
            highscore_table->addHighscore(stage->score);
            playing = false;
            stage->resetStage();
            for (auto &&key : keyboard)
            {
                key = false;
            }
        }
        else
        {
            stage->logic();
        }
    }
}

void App::draw()
{
    background->drawBackground();
    background->drawStarfield();

    if (!playing)
    {
        highscore_table->draw();
    }
    else
    {
        stage->draw();
    }
}

void App::doInput()
{
    SDL_Event event;

    inputText.clear();

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;

        case SDL_KEYDOWN:
            doKeyDown(&event.key);
            break;

        case SDL_KEYUP:
            doKeyUp(&event.key);
            break;

        case SDL_TEXTINPUT:
            inputText = event.text.text;
            break;

        default:
            break;
        }
    }
}

void App::doKeyDown(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        keyboard[event->keysym.scancode] = true;
    }
}

void App::doKeyUp(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        keyboard[event->keysym.scancode] = false;
    }
}