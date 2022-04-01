#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "include.h"


class Radar {
public:
	Radar(int x = 0, int y = 0, int w = PIXEL_WIDTH * 3, int h = PIXEL_HEIGHT * 3, int alt = 1, int range = 120);

	SDL_Point getPos();
	void setPos(int x, int y);

	void setRange(int x);
	int getRange();

	void setTexture(SDL_Texture* newTexture);
	SDL_Texture* getTexture();

	bool setRadarNumPos(int x, int y);

	int getRadarNumPosX();
	int getRadarNumPosY();

	SDL_Point getCenter();
	void setCenter(int x, int y);

	int getAltitude();
	void setAltitude(int alt);

	int getSearchAltitude();
	void setSearchAltitude(int alt);

	bool is_visible_pierce(double tg, int pixelHeight, double step);
	
	void calculate_and_set_range(int radarType, int targetType);

private:
	SDL_Texture* radarTexture;

	SDL_Rect pos;
	SDL_Point center;

	int numRadarX;
	int numRadarY;

	int range;
	int altitude;
	int searchAltitude;
};