#pragma once

#include "LTexture.h"
//#include "pixel.h"
#include "map.h"
#include "GuiWindow.h"

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
TTF_Font *gFont = NULL;

Map map;

void prog();

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gWindow = SDL_CreateWindow("Radar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                               SDL_WINDOW_SHOWN);

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL)
        printf("Error with renderer");
    else
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);


    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("Error with png 1");
    }
    if (TTF_Init() == -1)
        printf("Error with initialization ttf");
}

SDL_Texture *loadTexture(std::string path) {
    SDL_Texture *newTexture = NULL;

    SDL_Surface *loadedSurface = NULL;
    loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
        printf("Error with loading");

    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 255, 255));

    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (newTexture == NULL)
        printf("Error with texture %s", SDL_GetError());

    SDL_FreeSurface(loadedSurface);

    return newTexture;
}

void close() {


    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void loadMedia() {
    SDL_Texture *newTexture;
    newTexture = loadTexture("images/radar.png");
    map.radar.setTexture(newTexture);

}

int mainLoop() {
    SDL_Event e;

    bool is_need_render = true;

    SDL_Point radarPos = map.radar.getPos();
    map.setRadarPixel();

    map.markCoveredPixels();


    while (1) {
        if (is_need_render == true) {
            map.renderMap();
            is_need_render = false;
        }

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)
                return 0;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 0;
                        break;
                }
            }
            /*else if (e.type == SDL_KEYDOWN) {
                printf("\nx =%d y = %d", map.pixels[0][0].getPosX(), map.pixels[0][0].getPosY());
                switch (e.key.keysym.sym) {
                    case SDLK_w:
                        map.scaleMap(1);
                        break;

                    case SDLK_s:
                        map.scaleMap(2);
                        break;

                    case SDLK_UP:
                        map.offsetMap(1);
                        break;

                    case SDLK_DOWN:
                        map.offsetMap(2);
                        break;

                    case SDLK_LEFT:
                        map.offsetMap(3);
                        break;

                    case SDLK_RIGHT:
                        map.offsetMap(4);
                        break;
                }

                is_need_render = true;
            }*/
        }

    }
}

int menu() {
    while (1) {
        int x, y, searchAlt;
        printf("Enter radar's coordinates and current altitude: \n");
        printf("x = ");
        scanf_s("%d", &x);
        printf("y = ");
        scanf_s("%d", &y);
        printf("alt = ");
        scanf_s("%d", &searchAlt);

        if (x < 0 || x >= PIXEL_PER_WIDTH || y < 0 || y >= PIXEL_PER_HEIGHT || searchAlt < 1 || searchAlt > 6) {
            printf("\nError! Enter again\n");
            continue;
        } else {

            map.radar.setRadarNumPos(x, y);
            map.radar.setPos(x * PIXEL_WIDTH, y * PIXEL_HEIGHT);

            int altitude = map.pixels[x][y].getAltitude();
            map.radar.setAltitude(altitude);

            return 0;
        }

    }
}

double getTg(int pixelHeight, int radarHeight, double step) {
    double tg = 0;

    tg = ((double) pixelHeight - radarHeight) / step;
    //printf("\ntg = %", tg);

    return tg;
}

void prog(int radarType, float heightValue, int x, int y, int targetType, std::string filepath) {


    float floatAlt = heightValue;
    floatAlt *= 10;
    int searchAlt = (int) floatAlt;
    if (searchAlt < 2) {
        searchAlt = 2;
    }

    map.radar.setRadarNumPos(x, y);
    map.radar.setPos(x * PIXEL_WIDTH, y * PIXEL_HEIGHT);

    map.setFilePath(filepath);
    map.radar.calculate_and_set_range(radarType, targetType);

    int altitude = map.pixels[x][y].getAltitude();
    map.radar.setAltitude(altitude);

    map.radar.setSearchAltitude(searchAlt);
    map.setPixels();

    init();
//    loadMedia();
    mainLoop();
    close();
}

SimulationData getSimulationData() {
    GuiWindow window("Радар", 480, 400);

    window.render();
    return window.getSimulationData();
}

int main(int argc, char *args[]) {

    FreeConsole();
    SimulationData data;
    do {
        data = getSimulationData();

        if (data.close) {
            return 0;
        }

        if (data.isCorrect) {
            prog(data.radarType, data.heightValue, data.x, data.y, data.targetType, data.filePath);
        }
    } while (!data.close);
    return 0;
}
