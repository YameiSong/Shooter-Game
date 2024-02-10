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
    int y = 150;

    text->drawText(SCREEN_WIDTH / 2, 70, 255, 255, 255, ALIGN_CENTER, "%s", "HIGHSCORES");

    auto it = highscores.begin();

    int r, g, b;

    for (int i = 0; i < NUM_HIGHSCORES; i++, it++)
    {
        r = g = b = 255;
        if (it == recent)
        {
            b = 0;
        }

        text->drawText(SCREEN_WIDTH / 2, y, r, g, b, ALIGN_CENTER, "#%d. %-15s ...... %03d", i + 1, it->name.c_str(), it->score);

        y += 50;
    }

    text->drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, ALIGN_CENTER, "%s", "PRESS FIRE TO PLAY!");
}

void HighscoreTable::drawName()
{
    SDL_Rect r;

    text->drawText(SCREEN_WIDTH / 2, 70, 255, 255, 255, ALIGN_CENTER, "%s", "CONGRATULATIONS, YOU'VE GAINED A HIGHSCORE!");

    text->drawText(SCREEN_WIDTH / 2, 120, 255, 255, 255, ALIGN_CENTER, "%s", "ENTER YOUR NAME BELOW:");

    text->drawText(SCREEN_WIDTH / 2, 250, 128, 255, 128, ALIGN_CENTER, "%s", name.c_str());

    if (cursorBlink < FPS / 2)
    {
        r.x = SCREEN_WIDTH / 2 + name.size() * GLYPH_WIDTH / 2 + 5;
        r.y = 250;
        r.w = GLYPH_WIDTH;
        r.h = GLYPH_HEIGHT;

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &r);
    }

    text->drawText(SCREEN_WIDTH / 2, 625, 255, 255, 255, ALIGN_CENTER, "%s", "PRESS RETURN WHEN FINISHED");
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
        for (char &c : inputText)
        {
            c = std::toupper(c);
        }
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