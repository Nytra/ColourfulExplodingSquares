#include "ParticleEmitter.h"

int ParticleEmitter::inst_num = 0;

ParticleEmitter::ParticleEmitter()
{
	//inst_num = Emitter::inst_num;
	mInstNum = inst_num;
	inst_num += 1;
	mX = 0;
	mY = 0;
	mStartTime = SDL_GetTicks();
	//mParticles = new Particle[MAX_PARTICLES];
	mParticles = new Particle[MAX_PARTICLES];
	//mParticles = nullptr;
	mActive = false;
}

ParticleEmitter::~ParticleEmitter()
{
	mX = 0;
	mY = 0;
	mStartTime = 0;
	delete[] mParticles;
	mParticles = nullptr;
	mActive = false;
}

void ParticleEmitter::reset()
{
	printf("reset particle emitter %d at pos x %d y %d\n", mInstNum, mX, mY);
	for (int i = 0; i < MAX_PARTICLES; i++) {
		randomizeParticle(i);
	}
	mActive = true;
	mStartTime = SDL_GetTicks();
}

void ParticleEmitter::setPos(int x, int y)
{
	mX = x;
	mY = y;
}

Particle* ParticleEmitter::getParticles()
{
	return mParticles;
}

Uint32 ParticleEmitter::getStartTime()
{
	return mStartTime;
}

void ParticleEmitter::stop()
{
	mActive = false;
	//delete[] mParticles;
	//mParticles = nullptr;
}

//void ParticleEmitter::pause()
//{
	//mActive = false;
//}

bool ParticleEmitter::isActive()
{
	return mActive;
}

void ParticleEmitter::tick()
{
	bool particlesDone;
	if (isActive()) {
		//if (SDL_GetTicks() - getStartTime() > PARTICLE_LIFE_TIME) {
			//stop();
		//}
		//particles = gParticleEmitters[i].getParticles();
		particlesDone = true;
		for (int p = 0; p < MAX_PARTICLES; p++) {
			if (SDL_GetTicks() - mParticles[p].getStartTime() > PARTICLE_LIFE_TIME) {
				//randomizeParticle(p);
			}
			if (SDL_GetTicks() - mParticles[p].getStartTime() < mParticles[p].getDelay()) {
				//continue;
			}
			if (mParticles[p].getAlpha() == 0) {
				randomizeParticle(p);
			}
			mParticles[p].move();
			if (mParticles[p].getForce() > 1) {
				particlesDone = false;
			}
		}
		if (particlesDone) {
			stop();
		}
	}
}

void ParticleEmitter::randomizeParticle(int i)
{
	SDL_Rect r;
	SDL_Color c;

	r.x = mX;
	r.y = mY;
	r.w = (rand() % MAX_PARTICLE_SIZE) + 1;
	r.h = r.w;

	c.r = rand() % 0xFF;
	c.g = rand() % 0xFF;
	c.b = rand() % 0xFF;
	c.a = 0xFF;

	mParticles[i].setRect(r);
	mParticles[i].setImpulse(rand() % 360, (rand() % PARTICLE_MAX_SPEED) + 1);
	mParticles[i].setColor(c);
	mParticles[i].resetTime();
}

int ParticleEmitter::getXPos()
{
	return mX;
}

int ParticleEmitter::getYPos()
{
	return mY;
}

void ParticleEmitter::setActive(bool b)
{
	mActive = b;
}

void ParticleEmitter::resetStartTime()
{
	mStartTime = SDL_GetTicks();
}

int ParticleEmitter::getInstNum()
{
	return mInstNum;
}
