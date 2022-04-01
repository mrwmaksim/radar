#pragma once
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cmath>
#include <sstream>

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

const int PIXEL_WIDTH = 2;
const int PIXEL_HEIGHT = 2;

const int RADAR_PIXEL_STEP = 1;
const double RADAR_ANGLE_STEP = 0.5;

const int PIXEL_PER_WIDTH = 250;
const int PIXEL_PER_HEIGHT = 250;

const double PI = 3.14;

SDL_Texture* loadTexture(std::string path);

void init();

double getTg(int pixelHeight, int radarHeight, double step);
void prog(int radarHeight = 0, float heightValue = 0.150, int x = 10, int y = 10, int targetType = 0, std::string filepath = "files/input.txt");

