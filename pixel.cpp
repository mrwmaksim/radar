#include "pixel.h"

Pixel::Pixel(int x, int y, int w, int h, bool is_radar) {
	pos.x = x;
	pos.y = y;
	pos.w = w;
	pos.h = h;

	this->is_radar = is_radar;

	altitude = 1;
	is_covered = false;

	color = { 0, 255, 0, 0 };
}


int Pixel::getWidth() {
	return pos.w;
}

int Pixel::getHeight() {
	return pos.h;
}

void Pixel::setWidth(int w) {
	pos.w = w;
}

void Pixel::setHeight(int h) {
	pos.h = h;
}

int Pixel::getPosX() {
	return pos.x;
}

int Pixel::getPosY() {
	return pos.y;
}

int Pixel::getAltitude() {
	return altitude;
}

void Pixel::setAltitude(int alt) {

	altitude = alt;

	switch (alt) {
		case 1:
			color = { 0, 204, 0, 0};
			break;

		case 2:
			color = { 0, 255, 0, 0};
			break;

		case 3:
			color = { 51, 255, 51, 0};
			break;

		case 4:
			color = { 102, 255, 102, 0 };
			break;

		case 5:
			color = { 153, 255, 153, 0 };
			break;

		case 6:
			color = { 255, 255, 153, 0 };
			break;

		case 7:
			color = { 255, 255, 102, 0 };
			break;

		case 8:
			color = { 255, 255, 51, 0 };
			break;

		case 9:
			color = { 255, 153, 51, 0 };
			break;

		case 10:
			color = { 255, 128, 0, 0 };
			break;
	}
}

SDL_Point Pixel::getPos() {
	SDL_Point posPoint;

	posPoint.x = pos.x;
	posPoint.y = pos.y;
	
	return posPoint;
}

void Pixel::setPos(int x, int y) {
	pos.x = x;
	pos.y = y;
}

void Pixel::setRendererColor(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

SDL_Rect* Pixel::getRect() {
	return &pos;
}

void Pixel::setRadar() {
	is_radar = true;
	setCovered();
	setCoveredColor();
}

bool Pixel::getRadar() {
	return is_radar;
}

void Pixel::setCovered() {
	is_covered = true;
}

bool Pixel::getCovered() {
	setCoveredColor();
	return is_covered;
}

bool Pixel::is_over(int x, int y) {
	if (x < pos.x || x > (pos.x + PIXEL_WIDTH) || y < pos.y || y > (pos.y + PIXEL_HEIGHT))
		return false;
	else
		return true;
}

void Pixel::setCoveredColor() {
	
	switch (altitude) {
		case 1:
			color = { 153, 255, 255, 0 };
			break;

		case 2:
			color = { 102, 255, 255, 0 };
			break;

		case 3:
			color = { 51, 255, 255, 0 };
			break;

		case 4:
			color = { 0, 255, 255, 0 };
			break;

		case 5:
			color = { 0, 204, 204, 0 };
			break;

		case 6:
			color = { 51, 153, 255, 0 };
			break;

		case 7:
			color = { 0, 128, 255, 0 };
			break;

		case 8:
			color = { 0, 102, 204, 0 };
			break;

		case 9:
			color = { 0, 76, 153, 0 };
			break;

		case 10:
			color = { 0, 51, 102, 0 };
			break;

	}
}

void Pixel::setNotRadar() {
    is_radar = false;
}
