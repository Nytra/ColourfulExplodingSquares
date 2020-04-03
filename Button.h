#pragma once
#include <SDL.h>
#include <stdio.h>
#include <random>
#include "Source.h"
class Button
{
public:
	Button();
	~Button();

	int getXPos();
	int getYPos();
	int getWidth();
	int getHeight();
	int getDir();
	SDL_Color getColor();
	SDL_Rect getRect();

	bool setColor(SDL_Color color);
	bool setPos(int x, int y);
	bool setRect(SDL_Rect rect);
	bool setDir(int degrees);
	bool setActive(bool b);
	bool modulateDirection(int degrees);
	bool move();
	bool isActive();

	void goBack();

	//void explode();
private:
	int mX;
	int mY;
	int mW;
	int mH;
	SDL_Color mColor;
	int mDir;
	bool mActive;
	int mPrevX;
	int mPrevY;

	void _validateDirection();
};

