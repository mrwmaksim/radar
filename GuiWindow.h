#pragma once

#include <d3d9.h>
#include <string>

#include <imgui.h>
#include "../ImGui/examples/imgui_impl_dx9.h"
#include "../ImGui/examples/imgui_impl_win32.h"

struct SimulationData {
    int radarType = 0;
    float heightValue = 0;
    int x = 0;
    int y = 0;
    int targetType = 0;
    std::string filePath;
    bool isCorrect = false;
    bool close = false;
};

class GuiWindow {
private:

    ImFont *mainFont;
    ImFont *bigFont;

    SimulationData simulationData;

    static int width;
    static int height;

    bool isRunning;
    bool isMainMenu;
    bool isError;

    std::string errorMessage;

    std::string windowName;

    void createHWindow();

    void destroyHWindow();

    bool createDevice();

    void destroyDevice();

    void createImGui();

    void destroyImGui();

    void beginRender();

    void draw();

    void drawMainMenu();

    void endRender();

    void resetDevice();

    static long __stdcall WindowProcess(
            HWND window,
            UINT message,
            WPARAM wideParameter,
            LPARAM longParameter);

    // WIN
    static HWND window;
    static POINTS position;
    WNDCLASSEX windowClass = {};

    // D3D
    PDIRECT3D9 d3d = nullptr;
    LPDIRECT3DDEVICE9 device = nullptr;
    D3DPRESENT_PARAMETERS presentParameters = {};


    char szFile[512]; // WIN API variable
    std::string openFile();

public:

    GuiWindow(std::string name, int w, int h);

    ~GuiWindow();

    SimulationData getSimulationData();

    void render();

};

