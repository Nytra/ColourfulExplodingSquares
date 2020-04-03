#include "Message.h"

Message::Message()
{
	mText = "";
	mTexture = nullptr;
	mColor = nullptr;
	mX = 0;
	mY = 0;
	mW = 0;
	mH = 0;
}

Message::~Message()
{
	SDL_DestroyTexture(mTexture);
}

#if defined(SDL_TTF_H_)
void Message::generateFont(std::string text, SDL_Color* color)
{
	SDL_DestroyTexture(mTexture);
	mText = text;
	mColor = color;
	SDL_Surface* fontSurface = TTF_RenderText_Solid(gFont, text.c_str(), *color);
	mW = fontSurface->w;
	mH = fontSurface->h;
	mTexture = SDL_CreateTextureFromSurface(gRenderer, fontSurface);
	SDL_FreeSurface(fontSurface);
	fontSurface = nullptr;
}
#endif

void Message::render(int x, int y)
{
	mX = x;
	mY = y;
	SDL_Rect dstRect = { mX, mY, mW, mH };
	SDL_RenderCopy(gRenderer, mTexture, nullptr, &dstRect);
}
