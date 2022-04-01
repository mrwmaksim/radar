#pragma once
#include "include.h"


class LTexture {
public:

	LTexture();

	~LTexture();

	void loadFromFile(std::string path);

	void free();

	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0, SDL_Point* center = NULL, SDL_RendererFlip = SDL_FLIP_NONE);

	void loadFromRenderedText(std::string textureText, SDL_Color textColor);

	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);

	int getWidth();
	int getHeight();

private:

	SDL_Texture* mTexture;

	int mWidth;
	int mHeight;
};