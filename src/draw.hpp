#include "common.hpp"
#include <string>

void prepareScene();
void presentScene();
SDL_Texture *loadTexture(std::string filename);
void blit(SDL_Texture *texture, int x, int y);