#pragma once
#include "radar.h"
#include "include.h"

Radar::Radar(int x, int y, int w, int h, int alt, int range) {
	pos.x = x;
	pos.y = y;
	pos.w = w;
	pos.h = h;

	altitude = alt;
	searchAltitude = 1;

	this->range = range;

	numRadarY = 0;
	numRadarX = 0;

	radarTexture = NULL;

	center.x = PIXEL_WIDTH / 2;
	center.y = PIXEL_HEIGHT / 2;

}

void Radar::setPos(int x, int y) {
	pos.x = x;
	pos.y = y;

	center.x = x + PIXEL_WIDTH / 2;
	center.y = y + PIXEL_HEIGHT / 2;
}

SDL_Point Radar::getPos() {
	SDL_Point pos;
	pos.x = this->pos.x;
	pos.y = this->pos.y;

	return pos;
}

void Radar::setRange(int x) {
	range = x;
}

int Radar::getRange() {
	return range;
}

SDL_Texture* Radar::getTexture() {
	return radarTexture;
}

void Radar::setTexture(SDL_Texture* newTexture) {
	radarTexture = newTexture;
}

bool Radar::setRadarNumPos(int x, int y) {
	if (x >= 1 && x <= PIXEL_PER_WIDTH && y <= PIXEL_PER_HEIGHT && y >= 1) {
		numRadarX = x;
		numRadarY = y;
		return true;
	}
	else
		return false;
}

int Radar::getRadarNumPosX() {
	return numRadarX;
}

int Radar::getRadarNumPosY() {
	return numRadarY;
}

SDL_Point Radar::getCenter() {
	return center;
}

void Radar::setCenter(int x, int y) {
	center.x = x;
	center.y = y;
}

int Radar::getAltitude() {
	return altitude;
}

void Radar::setAltitude(int alt) {
	altitude = alt;
}

int Radar::getSearchAltitude() {
	return searchAltitude;
}

void Radar::setSearchAltitude(int alt) {
	searchAltitude = alt;
}
bool Radar::is_visible_pierce(double tg, int pixelHeight, double step) {

	double yBorder = tg * step + altitude;
	if ((double)pixelHeight >= yBorder)
		return true;
	else
		return false;
}

void Radar::calculate_and_set_range(int radarType, int targetType) {
	int range = 0;

	if (radarType == 0) {
		range = 130;
		if (targetType == 0) {
			range = 130;
		}
		if (targetType == 1) {
			range = 62;
		}
		if (targetType == 2) {
			range = 35;
		}
	}

	if (radarType == 1) {
		range = 100;
		if (targetType == 0) {
			range = 100;
		}
		if (targetType == 1) {
			range = 48;
		}
		if (targetType == 2) {
			range = 27;
		}
	}
	setRange(range);
}