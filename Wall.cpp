#include "Wall.h"

Wall::Wall()
{
	mSolid = true;
	mRect = nullptr;
}

Wall::~Wall()
{
}

void Wall::setRect(SDL_Rect* r)
{
	mRect = r;
}

void Wall::setSolid(bool b)
{
	mSolid = b;
}

bool Wall::isSolid()
{
	return mSolid;
}

SDL_Rect* Wall::getRect()
{
	return mRect;
}
