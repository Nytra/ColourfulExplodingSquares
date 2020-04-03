#pragma once
#include "ParticleEmitter.h"
#include "Particle.h"

class Explosion : public ParticleEmitter
{
public:
	Explosion(int force = 150);
	~Explosion();
	void tick();
	void reset();
	void setForce(int force);
private:
	int mForce;
};

