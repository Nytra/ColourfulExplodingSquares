#include "Button.h"

Button::Button()
{
	mX = 0;
	mY = 0;
	mW = 0;
	mH = 0;
	mColor = { 0, 0, 0 };
	mActive = true;
	//mParticles = nullptr;
	mDir = 0;
	//mExplodeTime = 0;
	mPrevX = 0;
	mPrevY = 0;
}

Button::~Button()
{
	mX = 0;
	mY = 0;
	mW = 0;
	mH = 0;
	mColor = { 0, 0, 0 };
	mActive = false;
	//mParticles = nullptr;
	mDir = 0;
	//mExplodeTime = 0;
	mPrevX = 0;
	mPrevY = 0;
}

int Button::getXPos()
{
	return mX;
}

int Button::getYPos()
{
	return mY;
}

int Button::getWidth()
{
	return mW;
}

int Button::getHeight()
{
	return mH;
}

int Button::getDir()
{
	return mDir;
}

SDL_Color Button::getColor()
{
	return mColor;
}

SDL_Rect Button::getRect()
{
	SDL_Rect r = { mX, mY, mW, mH };
	return r;
}

//void Button::getParticles(SDL_Rect* _out)
//{
//	_out = mParticles;
//}

//Uint32 Button::getExplodeTime()
//{
//	return SDL_GetTicks() - mExplodeTime;
//}

bool Button::setColor(SDL_Color color)
{
	mColor = color;
	return true;
}

bool Button::setPos(int x, int y)
{
	mX = x;
	mY = y;
	return true;
}

bool Button::setRect(SDL_Rect rect)
{
	mX = rect.x;
	mY = rect.y;
	mW = rect.w;
	mH = rect.h;
	return true;
}

bool Button::setDir(int degrees)
{
	mDir = degrees;
	_validateDirection();
	return true;
}

bool Button::setActive(bool b)
{
	mActive = b;
	//mExplodeTime = SDL_GetTicks();
	return true;
}

bool Button::modulateDirection(int degrees)
{
	mDir += degrees;
	//printf("mDir: %d\n", mDir);
	_validateDirection();
	return true;
}

bool Button::move()
{
	// 8 possible directions
	// so first quantize dir by dividing by 8?

	int quantized = round(mDir / 45.0);
	//printf("quantized: %d\n", quantized);

	if (quantized == 8) { // it should never be 8
		quantized = 0;
	}

	mPrevX = mX;
	mPrevY = mY;

	switch (quantized) {
	case 0:
		mY -= 1;
		break;
	case 1:
		mX += 1;
		mY -= 1;
		break;
	case 2:
		mX += 1;
		break;
	case 3:
		mX += 1;
		mY += 1;
		break;
	case 4:
		mY += 1;
		break;
	case 5:
		mX -= 1;
		mY += 1;
		break;
	case 6:
		mX -= 1;
		break;
	case 7:
		mX -= 1;
		mY -= 1;
		break;
	}

	if (mX > SCREEN_WIDTH - mW) {
		mX = SCREEN_WIDTH - mW;
	}
	else if (mX < 0) {
		mX = 0;
	}

	if (mY > SCREEN_HEIGHT - mH) {
		mY = SCREEN_HEIGHT - mH;
	}
	else if (mY < 0) {
		mY = 0;
	}

	return true;
}

bool Button::isActive()
{
	return mActive;
}

void Button::goBack()
{
	mX = mPrevX;
	mY = mPrevY;
}

//bool Button::hasParticles()
//{
//	return mExplodeTime != 0;
//}

//void Button::explode()
//{
//	int rX, rY, rW;
//	mParticles = new SDL_Rect[MAX_PARTICLES];
//	mExplodeTime = SDL_GetTicks();
//	for (int i = 0; i < MAX_PARTICLES; i++) {
//		rX = (rand() % mW) - (mW / 2);
//		rY = (rand() % mH) - (mH / 2);
//		rW = (rand() % MAX_PARTICLE_SIZE) + 1;
//		mParticles[i].x = rX;
//		mParticles[i].y = rY;
//		mParticles[i].w = rW;
//		mParticles[i].h = rW;
//		printf("created particle x %d y %d w %d h %d\n", rX, rY, rW, rW);
//	}
//	mActive = false;
//}

void Button::_validateDirection()
{
	if (mDir > 359) {
		mDir -= 360 * floor(mDir / 360.0);
	}
	else if (mDir < 0) {
		mDir += 360 * floor(abs(mDir) / 360.0);
		mDir = 360 - abs(mDir);
	}
	//printf("mDir: %d\n", mDir);
}