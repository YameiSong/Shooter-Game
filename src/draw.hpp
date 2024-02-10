#pragma once

#include "common.hpp"
#include <string>

void prepareScene(SDL_Renderer* renderer);
void presentScene(SDL_Renderer* renderer);
SDL_Texture *loadTexture(SDL_Renderer* renderer, std::string filename);
void blit(SDL_Renderer* renderer, SDL_Texture *texture, int x, int y);
void blitRect(SDL_Renderer* renderer, SDL_Texture *texture, SDL_Rect *src, int x, int y);
void capFrameRate(uint64_t &then, float &remainder);