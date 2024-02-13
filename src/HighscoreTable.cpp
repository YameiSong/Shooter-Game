#include "HighscoreTable.hpp"
#include "Stage.hpp"
#include "defs.hpp"
#include <algorithm>

extern bool keyboard[MAX_KEYBOARD_KEYS];

HighscoreTable::HighscoreTable(SDL_Renderer *renderer,
                               std::shared_ptr<Text> text) : recent(highscores.end()),
                                                             renderer(renderer),
                                                             text(text),
                                                             cursorBlink(0),
                                                             inputDone(false)
{
    name.clear();

    for (int i = 0; i < NUM_HIGHSCORES; i++)
    {
        highscores.push_back({0, ANONYMOUS});
    }
}

void HighscoreTable::logic(std::string &inputText)
{
    if (recent != highscores.end())
    {
        doNameInput(inputText);
    }

    if (++cursorBlink >= FPS)
    {
        cursorBlink = 0;
    }
}

void HighscoreTable::draw()
{
    if (recent != highscores.end() && !inputDone)
    {
        drawName();
    }
    else
    {
        drawHighscore();
    }
}

void HighscoreTable::drawHighscore()
{
    int y = 250;

    text->drawText(SCREEN_WIDTH / 2, 70, 255, 255, 255, FONT_TITLE, ALIGN_CENTER, "%s", "High Scores");

    auto it = highscores.begin();

    int r, g, b;

    for (int i = 0; i < NUM_HIGHSCORES; i++, it++)
    {
        r = g = b = 255;
        if (it == recent)
        {
            b = 0;
        }

        text->drawText(SCREEN_WIDTH / 2 - 200, y, r, g, b, FONT_COMMAND, ALIGN_LEFT, "#%d. %-15s", i + 1, it->name.c_str(), it->score);
        text->drawText(SCREEN_WIDTH / 2 + 50, y, r, g, b, FONT_COMMAND, ALIGN_LEFT, "............ %03d", it->score);

        y += 50;
    }

    text->drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, FONT_COMMAND, ALIGN_CENTER, "%s", "Press fire to play!");
}

void HighscoreTable::drawName()
{
    SDL_Rect r;

    text->drawText(SCREEN_WIDTH / 2, 70, 255, 255, 255, FONT_TITLE, ALIGN_CENTER, "%s", "Congratulations, you've gained a high score!");

    text->drawText(SCREEN_WIDTH / 2, 200, 255, 255, 255, FONT_COMMAND, ALIGN_CENTER, "%s", "Enter your name below:");

    text->drawText(SCREEN_WIDTH / 2, 300, 128, 255, 128, FONT_COMMAND, ALIGN_CENTER, "%s", name.c_str());

    // if (cursorBlink < FPS / 2)
    // {
    //     r.x = SCREEN_WIDTH / 2 + name.size() * GLYPH_WIDTH;
    //     r.y = 250;
    //     r.w = 5;
    //     r.h = GLYPH_HEIGHT;

    //     SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    //     SDL_RenderFillRect(renderer, &r);
    // }

    text->drawText(SCREEN_WIDTH / 2, 625, 255, 255, 255, FONT_COMMAND, ALIGN_CENTER, "%s", "Press return when finished");
}

void HighscoreTable::addHighscore(int score)
{
    inputDone = false;

    Highscore x{score, ANONYMOUS};

    auto position = std::lower_bound(highscores.begin(), highscores.end(), x, std::greater<Highscore>());
    if (position != highscores.end())
    {
        auto it = highscores.insert(position, x);
        recent = it;
        highscores.pop_back();
    }
    else
    {
        recent = highscores.end();
    }
}

void HighscoreTable::doNameInput(std::string &inputText)
{
    if (!inputText.empty())
    {
        name.append(inputText);
    }

    if (!name.empty() && keyboard[SDL_SCANCODE_BACKSPACE])
    {
        name.pop_back();
        keyboard[SDL_SCANCODE_BACKSPACE] = false;
    }

    if (keyboard[SDL_SCANCODE_RETURN] && !inputDone)
    {
        recent->name = name;
        name.clear();
        inputDone = true;
    }
}