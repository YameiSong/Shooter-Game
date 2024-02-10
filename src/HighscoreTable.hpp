#include "Text.hpp"
#include <list>

class HighscoreTable
{
private:

    std::list<int> highscores;
    std::list<int>::iterator recent;
    std::shared_ptr<Text> text;

public:
    HighscoreTable(std::shared_ptr<Text> text);
    ~HighscoreTable() = default;
    void draw();
    void drawHighscores();
    void addHighscore(int score);
};
