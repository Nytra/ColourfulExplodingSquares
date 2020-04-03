#pragma once
#include <SDL.h>
#include <random>
#include "Particle.h"
#include "Source.h"

class ParticleEmitter
{
public:
	static int inst_num;
	ParticleEmitter();
	~ParticleEmitter();
	void reset();
	void setPos(int x, int y);
	Particle* getParticles();
	Uint32 getStartTime();
	void stop();
	//void pause();
	bool isActive();
	void tick();
	void randomizeParticle(int i);
	int getXPos();
	int getYPos();
	void setActive(bool b);
	void resetStartTime();
	int getInstNum();
private:
	int mX, mY;
	Uint32 mStartTime;
	Particle* mParticles;
	bool mActive;
	int mInstNum;
};