#pragma once
#include <SDL.h>
#include <stdio.h>

class Particle
{
public:
	Particle();
	~Particle();
	void setImpulse(int angle, int force);
	void setRect(SDL_Rect r);
	void setColor(SDL_Color c);
	void setAlpha(int alpha);
	void modAlpha(int amt);
	void move();

	SDL_Rect getRect();
	SDL_Color getColor();
	int getAngle();
	int getForce();
	int getAlpha();
	int getInitForce();
	Uint32 getStartTime();
	void resetTime();
	Uint32 getDelay();
private:
	SDL_Rect mRect;
	int mForce, mAngle, mInitForce;
	SDL_Color mColor;
	int mAlpha;
	Uint32 mStartTime;
	Uint32 mDelay;
};

