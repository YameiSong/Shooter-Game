#pragma once

#include "common.hpp"
#include <string>

void prepareScene();
void presentScene();
SDL_Texture *loadTexture(std::string filename);
void blit(SDL_Texture *texture, int x, int y);
void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y);
void capFrameRate(uint64_t &then, float &remainder);