#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <random>
#include <time.h>
#include <sstream>
#include <vector>
#include "Button.h"
#include "Message.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "WavBuilder.h"
#include "Wall.h"
#include "Explosion.h"
#include "Source.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const bool DEBUG = false;

const int MAX_PARTICLES = 100; // per explosion
const int MAX_BUTTONS = 500;
const int MIN_BUTTON_SIZE = 5;
const int MAX_BUTTON_SIZE = 10;
const Uint32 PARTICLE_LIFE_TIME = 1000; // 1 second
const int PARTICLE_MAX_SPEED = 25; // pixels per update
const int MAX_PARTICLE_SIZE = 15;
const int MAX_PARTICLE_EMITTERS = 500;
const int GAME_LOGIC_UPDATE_RATE = 60; // times per second

const int SAMPLE_RATE = 22050;
const int AUDIO_CHANNELS = 1;
const int SAMPLE_SIZE = 1;

SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;
SDL_Renderer* gRenderer = nullptr;

Button gButtons[MAX_BUTTONS];
//ParticleEmitter gParticleEmitters[MAX_PARTICLE_EMITTERS];
Explosion gParticleEmitters[MAX_PARTICLE_EMITTERS];

//Button gParticles[MAX_PARTICLES];

TTF_Font* gFont = nullptr;
Message gText;
SDL_Color gWhite = { 255, 255, 255 };

Mix_Music* gMusic = nullptr;
Mix_Chunk* gSoundEffect = nullptr;

WavBuilder gWavBuilder;

//Wall gWallTest;
//Wall gWallTest2;
//Wall gWallTest3;
//Wall gLevelWalls[100];

struct Level {
	std::vector<Wall*> levelWalls;
	int nWalls = 0;
	void addWall(Wall* w) {
		levelWalls.push_back(w);
		nWalls += 1;
	}
};

Level gLevel1;
Level gLevel2;
Level* gLevels[] = { &gLevel1, &gLevel2 };
int gCurrentLevel = 0;

bool setup() {
	bool success = true;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	gWindow = SDL_CreateWindow("The Real Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	gScreenSurface = SDL_GetWindowSurface(gWindow);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	TTF_Init();
	gFont = TTF_OpenFont("OpenSans-Light.ttf", 12);
	Mix_OpenAudio(SAMPLE_RATE, AUDIO_U8, AUDIO_CHANNELS, SAMPLE_SIZE);

	return success;
}

bool generateButtons() {
	SDL_Rect buttonRect;
	SDL_Color buttonColor;
	bool goodPos;

	for (int i = 0; i < MAX_BUTTONS; i++) {
		buttonRect.w = (rand() % MAX_BUTTON_SIZE) + MIN_BUTTON_SIZE;
		buttonRect.h = buttonRect.w;
		while (true) {
			buttonRect.x = rand() % (SCREEN_WIDTH - buttonRect.w);
			buttonRect.y = rand() % (SCREEN_HEIGHT - buttonRect.h);
			goodPos = true;

			for (int w = 0; w < gLevels[gCurrentLevel]->nWalls; w++) {
				if (!gLevels[gCurrentLevel]->levelWalls.empty() && SDL_HasIntersection(&buttonRect, gLevels[gCurrentLevel]->levelWalls[w]->getRect())) {
					goodPos = false;
				}
			}

			if (goodPos) {
				break;
			}
		}
		gButtons[i].setRect(buttonRect);

		buttonColor = { (Uint8)(rand() % 0xFF), (Uint8)(rand() % 0xFF), (Uint8)(rand() % 0xFF), 128 };
		gButtons[i].setColor(buttonColor);

		gButtons[i].setDir(rand() % 360);
		gButtons[i].setActive(true);
	}

	return true;
}

void getPointByDirection(int x, int y, int length, int angle, SDL_Point* _out) {
	double rad;
	int x2, y2;
	rad = (angle) * 3.14 / 180.0;

	x2 = round(x + length * sin(rad)); // or do cos() for x and sin() for y and just subtract (3.14 / 2) from rad in both
	y2 = round(y + length * -cos(rad)); // because our angle starts at north and goes clockwise
	_out->x = x2;
	_out->y = y2;
}

void placeParticleEmitter(int x, int y) {
	for (int p = 0; p < MAX_PARTICLE_EMITTERS; p++) {
		if (!gParticleEmitters[p].isActive()) {
			gParticleEmitters[p].setPos(x, y);
			gParticleEmitters[p].reset();
			//printf("particle emitter %d reset to pos x %d y %d\n", p, x, y);
			break;
		}
	}
}

void stopAllParticleEmitters() {
	for (int i = 0; i < MAX_PARTICLE_EMITTERS; i++) {
		gParticleEmitters[i].stop();
	}
}

int main(int argc, char** argv) {
	setup();
	srand((unsigned)time(0));

	Wall* wall;
	SDL_Rect* wallRect;
	int x, y, w, h;
	bool okay = true;
	bool xLeft = true;
	bool yUp = true;
	int xMod = 1;
	int yMod = 1;
	bool xModUp = true;
	bool yModUp = true;
	for (int i = 0; i < 100; i++) {
		printf("%f %\n", i / 100.0);
		while (true) {
			w = (rand() % (int)(SCREEN_WIDTH * 0.1 - 10)) + 10;
			//h = (rand() % (int)(SCREEN_HEIGHT * 0.1)) + 20;
			h = w;
			x = rand() % (SCREEN_WIDTH - w);
			y = rand() % (SCREEN_HEIGHT - h);
			wall = new Wall;
			wallRect = new SDL_Rect{ x, y, w, h };

			for (int w = 0; w < gLevel1.nWalls; w++) {
				if (SDL_HasIntersection(wallRect, gLevel1.levelWalls[w]->getRect())) {
					okay = false;
					while (SDL_HasIntersection(wallRect, gLevel1.levelWalls[w]->getRect())) {
					start:
						if (xLeft) {
							wallRect->x -= xMod;
						}
						else {
							wallRect->x += xMod;
						}
						if (wallRect->x < 0) {
							wallRect->x = 0;
							xLeft = false;
						}
						if (wallRect->x > SCREEN_WIDTH - 1 - wallRect->w) {
							wallRect->x = SCREEN_WIDTH - 1 - wallRect->w;
							xLeft = true;
							if (xModUp) {
								xMod += rand() % 2;
							}
							else {
								xMod -= rand() % 2;
							}
							if (xMod > SCREEN_WIDTH - 1) {
								xModUp = false;
							}
							if (xMod < -SCREEN_WIDTH + 1) {
								xModUp = true;
							}
						}

						if (yUp) {
							wallRect->y -= yMod;
						}
						else {
							wallRect->y += yMod;
						}
						if (wallRect->y < 0) {
							wallRect->y = 0;
							yUp = false;
						}
						if (wallRect->y > SCREEN_HEIGHT - 1 - wallRect->h) {
							wallRect->y = SCREEN_HEIGHT - 1 - wallRect->h;
							yUp = true;
							if (yModUp) {
								yMod += rand() % 2;
							}
							else {
								yMod -= rand() % 2;
							}
							if (yMod > SCREEN_HEIGHT - 1) {
								yModUp = false;
							}
							if (yMod < -SCREEN_HEIGHT + 1) {
								yModUp = true;
							}
						}
						for (int w2 = 0; w2 < gLevel1.nWalls; w2++) {
							if (SDL_HasIntersection(wallRect, gLevel1.levelWalls[w2]->getRect())) {
								goto start;
							}
						}
						okay = true;
						break;
					}
				}
			}
			if (okay) {
				break;
			}

			//printf("failed\n");

			delete wall;
			delete wallRect;
		}

		wall->setRect(wallRect);
		gLevel1.addWall(wall);
	}

	
	//SDL_Rect wallRect2 = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3 };
	//SDL_Rect wallRect3 = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 10, SCREEN_HEIGHT / 10 };
	//gWallTest3.setRect(&wallRect3);
	//gWallTest2.setRect(&wallRect2);
	//gWallTest.setRect(&wallRect);

	//gLevel1.addWall(&gWallTest);
	//gLevel1.addWall(&gWallTest2);
	//gLevel2.addWall(&gWallTest3);

	printf("Level 1 nWalls: %d\n", gLevel1.nWalls);
	printf("Level 2 nWalls: %d\n", gLevel2.nWalls);

	for (int i = 0; i < gLevel1.nWalls; i++) {
		printf("Wall %d x %d y %d w %d h %d\n", i, gLevel1.levelWalls[i]->getRect()->x, gLevel1.levelWalls[i]->getRect()->y,
			gLevel1.levelWalls[i]->getRect()->w, gLevel1.levelWalls[i]->getRect()->h);
	}

	for (int i = 0; i < gLevel2.nWalls; i++) {
		printf("Wall %d x %d y %d w %d h %d\n", i, gLevel2.levelWalls[i]->getRect()->x, gLevel2.levelWalls[i]->getRect()->y,
			gLevel2.levelWalls[i]->getRect()->w, gLevel2.levelWalls[i]->getRect()->h);
	}

	//gLevelWalls[0] = gWallTest;
	//gLevelWalls[1] = gWallTest2;
	SDL_Rect wallRenderRect;

	generateButtons();

	bool quit = false;
	SDL_Event e;

	SDL_Color buttonColor;

	Uint32 startTime = 0;

	int mX, mY;
	SDL_Point mousePos;
	bool mousePressed = false;

	SDL_Point p1, p2;
	int lineLength = 10;

	//SDL_Point p3;
	SDL_Rect particleRect;
	Particle* particles;
	SDL_Color particleColor;

	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);

	bool particlesDone = false;
	int particleAlpha;

	gMusic = Mix_LoadMUS("music.wav");

	//gWavBuilder.build();

	//gSoundEffect = Mix_LoadWAV("sound.wav");
	//if (gSoundEffect == NULL) {
		//printf("Unable to load sound.wav Error: %s\n", Mix_GetError());
	//}

	//Mix_SetPostMix(processAudioStream, NULL);
	std::stringstream ss;
	ss.str("");

	Uint32 frameTicks = 0;
	int framesPresented = 0;
	float frameRate = 0;

	ParticleEmitter mouseEmitter;
	//Explosion mouseEmitter(150);

	float time;

	
	const SDL_Color gWallColor = { 96, 96, 96, 255};

	while (!quit) {
		while (SDL_PollEvent(&e) != NULL) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_p:
					generateButtons();
					stopAllParticleEmitters();
					break;
				case SDLK_m:
					// place particle emitter at mouse position
					SDL_GetMouseState(&mX, &mY);
					mousePos.x = mX;
					mousePos.y = mY;
					//placeParticleEmitter(mousePos.x, mousePos.y);
					mouseEmitter.setPos(mousePos.x, mousePos.y);
					if (!mouseEmitter.isActive()) {
						mouseEmitter.reset();
					}
					break;
				case SDLK_o:
					if (Mix_PlayingMusic() == 0) {
						Mix_PlayMusic(gMusic, 0);
					}
					else {
						if (Mix_PausedMusic()) {
							Mix_ResumeMusic();
						}
						else {
							Mix_PauseMusic();
						}
					}
					break;
				case SDLK_e:
					//Uint8 mem = 0x01001011;
					//gSoundEffect = Mix_QuickLoad_RAW(&mem, sizeof(mem));
					//Mix_HaltChannel(1);
					//Mix_
					//Mix_Volume(-1, 128);
					
					time = 2;
					char* buf;
					//gWavBuilder.build(time, 3000);
					buf = gWavBuilder.getWavInMem(time, 0);
					//for (int i = 0; i < SAMPLE_RATE * time; i++) {
						//printf("%d\n", (int)buf[i]);
					//}
					gSoundEffect = Mix_QuickLoad_RAW((Uint8*)buf, SAMPLE_RATE * time);
					//gSoundEffect = Mix_LoadWAV("sound.wav");
					Mix_PlayChannel(-1, gSoundEffect, 0);
					
					break;
				case SDLK_k:
					for (int i = 0; i < MAX_PARTICLE_EMITTERS; i++) {
						gParticleEmitters[i].stop();
					}
					for (int i = 0; i < MAX_BUTTONS; i++) {
						gButtons[i].setActive(false);
						placeParticleEmitter(gButtons[i].getXPos() + (gButtons[i].getWidth() / 2), gButtons[i].getYPos() + (gButtons[i].getHeight() / 2));
					}
					break;
				case SDLK_l:
					if (gCurrentLevel < sizeof(gLevels) / sizeof(*gLevels) - 1) {
						gCurrentLevel += 1;
					}
					else {
						gCurrentLevel = 0;
					}
					break;
				}
			}
			if (e.type == SDL_KEYUP) {
				switch (e.key.keysym.sym) {
				case SDLK_m:
					mouseEmitter.stop();
					break;
				}
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mX, &mY);
				mousePos.x = mX;
				mousePos.y = mY;
				mousePressed = true;
			}
			if (e.type == SDL_MOUSEBUTTONUP) {
				mousePos.x = -1;
				mousePos.y = -1;
				mousePressed = false;
			}
		}

		// RENDER STUFF

		SDL_SetRenderDrawColor(gRenderer, 128, 128, 128, 0xFF);
		SDL_RenderClear(gRenderer);

		for (int i = 0; i < MAX_BUTTONS; i++) {
			if (!gButtons[i].isActive()) {
				continue;
			}

			if (SDL_PointInRect(&mousePos, &gButtons[i].getRect()) && mousePressed) {
				gButtons[i].setActive(false);
				placeParticleEmitter(gButtons[i].getXPos() + (gButtons[i].getWidth() / 2), gButtons[i].getYPos() + (gButtons[i].getHeight() / 2));
			}
			
			buttonColor = gButtons[i].getColor();
			SDL_SetRenderDrawColor(gRenderer, buttonColor.r, buttonColor.g, buttonColor.b, 128);
			SDL_RenderFillRect(gRenderer, &gButtons[i].getRect());

			if (DEBUG) {

				p1.x = gButtons[i].getXPos();
				p1.y = gButtons[i].getYPos();
				getPointByDirection(p1.x, p1.y, lineLength, gButtons[i].getDir(), &p2);
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawLine(gRenderer, p1.x, p1.y, p2.x, p2.y);

				ss.str("");
				ss << "Angle: " << std::to_string(gButtons[i].getDir());
				gText.generateFont(ss.str().c_str(), &gWhite);
				gText.render(gButtons[i].getXPos(), gButtons[i].getYPos());

			}
		}

		for (int p = 0; p < MAX_PARTICLE_EMITTERS; p++) {
			if (gParticleEmitters[p].isActive()) {
				particles = gParticleEmitters[p].getParticles();
				for (int j = 0; j < MAX_PARTICLES; j++) {
					particleAlpha = (particles[j].getForce() / (double)particles[j].getInitForce()) * 255.0;
					if (particles[j].getForce() == 1) {
						particles[j].setAlpha(0);
						continue;
					}
					else {
						particles[j].setAlpha(particleAlpha);
					}
					
					particleRect = particles[j].getRect();

					if (particleRect.x < 0 || particleRect.x + particleRect.w >= SCREEN_WIDTH) {
						continue;
					}
					else if (particleRect.y < 0 || particleRect.y + particleRect.h >= SCREEN_HEIGHT) {
						continue;
					}

					particleColor = particles[j].getColor();
					particleAlpha = particles[j].getAlpha();
					SDL_SetRenderDrawColor(gRenderer, particleColor.r, particleColor.g, particleColor.b, particleAlpha);
					SDL_RenderFillRect(gRenderer, &particleRect);
					//printf("p %d x %d y %d angle %d force %d\n", j, particleRect.x, particleRect.y, particles[j].getAngle(),
						//particles[j].getForce());
				}
				//printf("particle emitter %d start time %d\n", p, gParticleEmitters[p].getStartTime());
			}
		}

		
		for (int w = 0; w < gLevels[gCurrentLevel]->nWalls; w++) {
			if (!gLevels[gCurrentLevel]->levelWalls.empty()) {
				wallRenderRect = *gLevels[gCurrentLevel]->levelWalls[w]->getRect();
				SDL_SetRenderDrawColor(gRenderer, gWallColor.r, gWallColor.g, gWallColor.b, 255);
				SDL_RenderFillRect(gRenderer, &wallRenderRect);
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &wallRenderRect);
			}
			
		}

		// END RENDER STUFF

		if (mouseEmitter.isActive()) {
			particles = mouseEmitter.getParticles();
			for (int i = 0; i < MAX_PARTICLES; i++) {
				particleAlpha = (particles[i].getForce() / (double)particles[i].getInitForce()) * 255.0;
				if (particles[i].getForce() == 1) {
					particles[i].setAlpha(0);
					continue;
				}
				else {
					particles[i].setAlpha(particleAlpha);
				}

				particleRect = particles[i].getRect();

				if (particleRect.x < 0 || particleRect.x + particleRect.w >= SCREEN_WIDTH) {
					continue;
				}
				else if (particleRect.y < 0 || particleRect.y + particleRect.h >= SCREEN_HEIGHT) {
					continue;
				}

				particleColor = particles[i].getColor();
				particleAlpha = particles[i].getAlpha();
				SDL_SetRenderDrawColor(gRenderer, particleColor.r, particleColor.g, particleColor.b, particleAlpha);
				SDL_RenderFillRect(gRenderer, &particleRect);
			}
		}
		

		if (mousePressed) {
			SDL_GetMouseState(&mX, &mY);
			mousePos.x = mX;
			mousePos.y = mY;
		}

		if (mouseEmitter.isActive()) {
			SDL_GetMouseState(&mX, &mY);
			mousePos.x = mX;
			mousePos.y = mY;
			mouseEmitter.setPos(mousePos.x, mousePos.y);
		}

		// GAME LOGIC
		if (SDL_GetTicks() - startTime > 1000 / GAME_LOGIC_UPDATE_RATE) {

			for (int i = 0; i < MAX_BUTTONS; i++) {
				if (gButtons[i].isActive()) {
					gButtons[i].modulateDirection(((rand() % 3) - 1) * 15);
					gButtons[i].move(); // make them avoid walls
					for (int w = 0; w < gLevels[gCurrentLevel]->nWalls; w++) {
						
						if (!gLevels[gCurrentLevel]->levelWalls.empty() && SDL_HasIntersection(&gButtons[i].getRect(), gLevels[gCurrentLevel]->levelWalls[w]->getRect())) {
							gButtons[i].goBack();
						}
					}
				}
			}

			for (int i = 0; i < MAX_PARTICLE_EMITTERS; i++) {
				gParticleEmitters[i].tick();
			}

			mouseEmitter.tick();

			startTime = SDL_GetTicks();
		}

		// CALCULATE FRAME RATE
		if (SDL_GetTicks() - frameTicks >= 1000) {
			frameTicks = SDL_GetTicks();
			frameRate = framesPresented;
			framesPresented = 0;
		}
		else {
			framesPresented += 1;
		}

		ss.str("");
		ss << "FPS: " << frameRate;
		gText.generateFont(ss.str().c_str(), &gWhite);
		gText.render(0, 0);

		//for (int i = 0; i < MAX_PARTICLES; i++) {
			//SDL_RenderFillRect(gRenderer, gParticles[i].getRect());
		//}
		
		SDL_RenderPresent(gRenderer);
	}

	return 0;
}