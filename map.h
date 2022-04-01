#pragma once
#include <SDL.h>
#include <fstream>
#include "pixel.h"
#include "radar.h"
#include "include.h"

class Map {
public:
	Map();
	~Map();

	void setRadarPixel();
	void markCoveredPixels();

	void getMatrixFromFile();

	void renderMap();

	void scaleMap(int mod);
	void offsetMap(int mod);

	void setPixels();
	void setFilePath(std::string path = "input.txt");

	Pixel** pixels;
	Radar radar;

private:
	std::string filepath;
	SDL_Point currentLeftTopAngle;

	int** altMatrix;//[PIXEL_PER_WIDTH][PIXEL_PER_HEIGHT];
};