#include "Particle.h"

Particle::Particle()
{
	mRect = { 0, 0, 0, 0 };
	mForce = 1;
	mAngle = 0;
	mColor = { 0, 0, 0 };
	mAlpha = 255;
	mInitForce = 0;
	mStartTime = SDL_GetTicks();
}

Particle::~Particle()
{
	mRect = { 0, 0, 0, 0 };
	mForce = 1;
	mAngle = 0;
	mColor = { 0, 0, 0 };
	mAlpha = 0;
	mStartTime = 0;
}

void Particle::setImpulse(int angle, int force)
{
	mAngle = angle;
	mForce = force;
	mInitForce = force;
	mDelay = 1000 / (double) force;
}

void Particle::setRect(SDL_Rect r)
{
	mRect.x = r.x;
	mRect.y = r.y;
	mRect.w = r.w;
	mRect.h = r.h;
}

void Particle::setColor(SDL_Color c)
{
	mColor = c;
}

void Particle::setAlpha(int alpha)
{
	mAlpha = alpha;
}

void Particle::modAlpha(int amt)
{
	mAlpha += amt;
}

void Particle::move()
{
	SDL_Point p;
	p.x = mRect.x + (mForce * sin(mAngle * 3.14 / 180.0));
	p.y = mRect.y + (mForce * -cos(mAngle * 3.14 / 180.0));
	mRect.x = p.x;
	mRect.y = p.y;

	if (mForce > 1) {
		mForce -= 1;
	}

	//if (mForce <= 0) {
		//mForce = 1;
	//}

	if (mAngle > 180.0) {
		mAngle -= 1 * (10 / mForce);
	}
	else if (mAngle < 180.0) {
		mAngle += 1 * (10 / mForce);
	}
}

SDL_Rect Particle::getRect()
{
	return mRect;
}

SDL_Color Particle::getColor()
{
	return mColor;
}

int Particle::getAngle()
{
	return mAngle;
}

int Particle::getForce()
{
	return mForce;
}

int Particle::getAlpha()
{
	return mAlpha;
}

int Particle::getInitForce()
{
	return mInitForce;
}

Uint32 Particle::getStartTime()
{
	return mStartTime;
}

void Particle::resetTime()
{
	mStartTime = SDL_GetTicks();
}

Uint32 Particle::getDelay()
{
	return mDelay;
}
