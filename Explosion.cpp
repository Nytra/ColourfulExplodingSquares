#include "Explosion.h"

Explosion::Explosion(int force)
{
	mForce = force;
}

Explosion::~Explosion()
{
	mForce = 0;
}

void Explosion::tick()
{
	bool particlesDone;
	Particle* particles = getParticles();

	if (isActive()) {
		//if (SDL_GetTicks() - getStartTime() > PARTICLE_LIFE_TIME) {
			//stop();
		//}

		particlesDone = true;
		for (int p = 0; p < MAX_PARTICLES; p++) {

			//if (SDL_GetTicks() - particles[p].getStartTime() > PARTICLE_LIFE_TIME) {
				//particles[p].setAlpha(0);
			//}

			particles[p].move();
			//if (particles[p].getForce() > 1) {
				//particlesDone = false;
			//}
			if (particles[p].getAlpha() > 0) {
				particlesDone = false;
			}
		}

		if (particlesDone) {
			stop();
		}
	}
}

void Explosion::reset()
{
	SDL_Rect r;
	SDL_Color c;
	Particle* particles = getParticles();
	printf("reset particle emitter %d at pos x %d y %d\n", getInstNum(), getXPos(), getYPos());
	for (int i = 0; i < MAX_PARTICLES; i++) {
		

		r.x = getXPos();
		r.y = getYPos();
		r.w = (rand() % MAX_PARTICLE_SIZE) + 1;
		r.h = r.w;

		c.r = rand() % 0xFF;
		c.g = rand() % 0xFF;
		c.b = rand() % 0xFF;
		c.a = 0xFF;

		particles[i].setRect(r);
		particles[i].setImpulse(rand() % 360, mForce * (double)(1 / (double)r.w));
		particles[i].setColor(c);
		particles[i].resetTime();
	}
	setActive(true);
	resetStartTime();
}

void Explosion::setForce(int force)
{
	mForce = force;	
}
