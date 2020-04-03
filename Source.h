#pragma once
#include <SDL_ttf.h>
#include <SDL.h>
//#include "Button.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern TTF_Font* gFont;
extern SDL_Renderer* gRenderer;
extern const int MAX_PARTICLES;
extern const int MAX_PARTICLE_SIZE;
extern const Uint32 PARTICLE_LIFE_TIME;
extern const int PARTICLE_MAX_SPEED;
//extern Button* gParticles[MAX_PARTICLES];