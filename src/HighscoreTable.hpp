#include "Text.hpp"
#include <list>
#include <string>

class HighscoreTable
{
private:
    struct Highscore
    {
        int score;
        std::string name;

        bool operator>(const Highscore &other) const
        {
            return score > other.score;
        }
    };
    std::string name;
    int cursorBlink;
    bool inputDone;
    std::list<Highscore> highscores;
    std::list<Highscore>::iterator recent;
    std::shared_ptr<Text> text;
    SDL_Renderer *renderer;
    void doNameInput(std::string &inputText);
    void drawHighscore();
    void drawName();

public:
    HighscoreTable(SDL_Renderer *renderer, std::shared_ptr<Text> text);
    ~HighscoreTable() = default;
    void logic(std::string &inputText);
    void draw();

    void addHighscore(int score);
};
