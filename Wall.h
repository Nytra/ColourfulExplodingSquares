#pragma once
#include <SDL.h>

class Wall
{
public:
	Wall();
	~Wall();
	void setRect(SDL_Rect* r);
	void setSolid(bool b);

	bool isSolid();
	SDL_Rect* getRect();
private:
	SDL_Rect* mRect;
	bool mSolid;
};

