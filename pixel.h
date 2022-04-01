#pragma once
#include "include.h"
#include <SDL.h>

class Pixel {
public:

	Pixel(int x = 0, int y = 0, int w = PIXEL_WIDTH, int h = PIXEL_HEIGHT, bool is_radar = false);

	int getWidth();
	int getHeight();

	void setWidth(int w);
	void setHeight(int h);

	int getAltitude();
	void setAltitude(int alt);

	SDL_Point getPos();
	int getPosX();
	int getPosY();
	void setPos(int x, int y);

	void setRendererColor(SDL_Renderer* renderer);

	SDL_Rect* getRect();

	void setCovered();
	bool getCovered();

	void setRadar();
    void setNotRadar();
	bool getRadar();

	bool is_over(int x, int y);

	void setCoveredColor();

private:
	SDL_Color color;

	int altitude;

	bool is_radar;
	bool is_covered;

	SDL_Rect pos;
	
};