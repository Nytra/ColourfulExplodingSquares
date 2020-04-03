#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Source.h"

class Message
{
public:
	Message();
	~Message();
#if defined(SDL_TTF_H_)
	void generateFont(std::string text, SDL_Color* color);
#endif
	void render(int x, int y);

private:
	SDL_Texture* mTexture;
	SDL_Color* mColor;
	std::string mText;
	int mX, mY;
	int mW, mH;
};

