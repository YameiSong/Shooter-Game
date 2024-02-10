#include "HighscoreTable.hpp"
#include "Stage.hpp"
#include "defs.hpp"
#include <algorithm>

HighscoreTable::HighscoreTable(std::shared_ptr<Text> text) : recent(highscores.end()),
                                                             text(text)
{
    for (int i = 0; i < NUM_HIGHSCORES; i++)
    {
        highscores.emplace_back(0);
    }
}

void HighscoreTable::draw()
{
    drawHighscores();
}

void HighscoreTable::drawHighscores()
{
    int y = 150;

    text->drawText(425, 70, 255, 255, 255, "%s", "HIGHSCORES");

    auto it = highscores.begin();

    for (int i = 0; i < NUM_HIGHSCORES; i++, it++)
    {
        if (it == recent)
        {
            text->drawText(425, y, 255, 255, 0, "#%d ............. %03d", i + 1, *it);
        }
        else
        {
            text->drawText(425, y, 255, 255, 255, "#%d ............. %03d", i + 1, *it);
        }

        y += 50;
    }

    text->drawText(425, 600, 255, 255, 255, "%s", "PRESS FIRE TO PLAY!");
}

void HighscoreTable::addHighscore(int score)
{
    auto position = std::lower_bound(highscores.begin(), highscores.end(), score, std::greater<int>());
    auto it = highscores.insert(position, score);
    recent = it;
    highscores.pop_back();
}