

#include "GuiWindow.h"


#include <iostream>

#include <utility>
#include <thread>


HWND GuiWindow::window = nullptr;
POINTS GuiWindow::position = {};
int GuiWindow::width = 640;
int GuiWindow::height = 480;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
        HWND window,
        UINT message,
        WPARAM wideParameter,
        LPARAM longParameter
);


GuiWindow::GuiWindow(std::string name, int w, int h) {
    windowName = std::move(name);
    width = w;
    height = h;

    isError = false;
    isMainMenu = true;

    simulationData.close = false;

    createHWindow();
    createDevice();
    createImGui();
}

void GuiWindow::createHWindow() {
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_CLASSDC;
    windowClass.lpfnWndProc = reinterpret_cast<WNDPROC>(WindowProcess);
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandleA(0);
    windowClass.hIcon = 0;
    windowClass.hCursor = 0;
    windowClass.hbrBackground = 0;
    windowClass.lpszMenuName = 0;
    windowClass.lpszClassName = "class001";
    windowClass.hIconSm = 0;

    RegisterClassEx(&windowClass);

    window = CreateWindowEx(
            0,
            "class001",
            windowName.c_str(),
            WS_POPUP,
            100,
            100,
            GuiWindow::width,
            GuiWindow::height,
            0,
            0,
            windowClass.hInstance,
            0
    );

    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);
}

bool GuiWindow::createDevice() {
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (!d3d)
        return false;

    ZeroMemory(&presentParameters, sizeof(presentParameters));

    presentParameters.Windowed = TRUE;
    presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    presentParameters.EnableAutoDepthStencil = TRUE;
    presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
    presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (d3d->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            window,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &presentParameters,
            &device) < 0)
        return false;

    return true;
}

void GuiWindow::createImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ::ImGui::GetIO();

    // Init font
    static const ImWchar ranges[] =
            {
                    0x0020, 0x00FF, // Basic Latin + Latin Supplement
                    0x0400, 0x044F, // Cyrillic
                    0,
            };
    ImFontConfig font_config;
    font_config.OversampleH = 1; //or 2 is the same
    font_config.OversampleV = 1;
    font_config.PixelSnapH = 1;
    mainFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 14.0f, &font_config, ranges);
    bigFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 48.0f, &font_config, ranges);

    io.IniFilename = NULL;

    ImGui::StyleColorsDark();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(64, 64, 64, 255));

    // Top Bar active
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(32, 32, 32, 255));

    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(32, 32, 32, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(32, 32, 32, 96));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(16, 16, 16, 255));

    // Radio
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(32, 32, 32, 255));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(0, 0, 0, 255));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(32, 32, 32, 255));
    ImGui::PushStyleColor(ImGuiCol_CheckMark, IM_COL32(128, 128, 128, 255));

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(device);
}

void GuiWindow::destroyImGui() {
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void GuiWindow::destroyDevice() {
    if (device) {
        device->Release();
        device = nullptr;
    }

    if (d3d) {
        d3d->Release();
        d3d = nullptr;
    }
}

void GuiWindow::destroyHWindow() {
    DestroyWindow(window);
    UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

void GuiWindow::beginRender() {
    MSG message;
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);

        if (message.message == WM_QUIT) {
            simulationData.close = true;
            isRunning = false;
            return;
        }
    }

    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void GuiWindow::render() {
    isRunning = true;
    while (isRunning) {

        beginRender();

        if (isRunning) {

            // DRAW
            if (isMainMenu) {
                drawMainMenu();
            } else {
                draw();
            }

            endRender();

            ImGuiIO &io = ImGui::GetIO();
            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
                isMainMenu = true;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        }
    }
}

void GuiWindow::endRender() {
    ImGui::EndFrame();

    device->SetRenderState(D3DRS_ZENABLE, FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

    if (device->BeginScene() >= 0) {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        device->EndScene();
    }

    const auto result = device->Present(0, 0, 0, 0);

    // Handle loss of D3D9 device
    if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
        resetDevice();
}

void GuiWindow::resetDevice() {
    ImGui_ImplDX9_InvalidateDeviceObjects();

    const auto result = device->Reset(&presentParameters);

    if (result == D3DERR_INVALIDCALL)
        IM_ASSERT(0);

    ImGui_ImplDX9_CreateDeviceObjects();
}

GuiWindow::~GuiWindow() {
    isRunning = false;
    destroyImGui();
    destroyDevice();
    destroyHWindow();
}

long GuiWindow::WindowProcess(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter) {
    if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
        return true;

    switch (message) {

        case WM_LBUTTONDOWN: {
            GuiWindow::position = MAKEPOINTS(longParameter); // set click points
        }
            return 0;

        case WM_MOUSEMOVE: {
            if (wideParameter == MK_LBUTTON) {
                const auto points = MAKEPOINTS(longParameter);
                auto rect = ::RECT{};

                GetWindowRect(GuiWindow::window, &rect);

                rect.left += points.x - GuiWindow::position.x;
                rect.top += points.y - GuiWindow::position.y;

                if (GuiWindow::position.x >= 0 &&
                    GuiWindow::position.x <= GuiWindow::width &&
                    GuiWindow::position.y >= 0 && GuiWindow::position.y <= 19)
                    SetWindowPos(
                            GuiWindow::window,
                            HWND_TOPMOST,
                            rect.left,
                            rect.top,
                            0, 0,
                            SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
                    );
            }

        }
            return 0;

        case WM_DESTROY: {
            PostQuitMessage(0);
        }
            return 0;

    }

    return DefWindowProc(window, message, wideParameter, longParameter);

}

void GuiWindow::draw() {
    if (!isRunning) return;


    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({(float) GuiWindow::width, (float) GuiWindow::height});
    ImGui::Begin(
            windowName.c_str(),
            &isRunning,
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoCollapse
    );

    // Radar type
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Выберите радар:55Ж6УМЕР-37М").x -
                          ImGui::GetStyle().ItemSpacing.x * 3) / 2);
    ImGui::Text("Выберите радар:");

    ImGui::SameLine();
    ImGui::RadioButton("55Ж6УМЕ", &simulationData.radarType, 0);

    ImGui::SameLine();
    ImGui::RadioButton("Р-37М", &simulationData.radarType, 1);

    ImGui::NewLine();

    // Radar position
    const int MAX_X = 249;
    const int MAX_Y = 249;

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Координаты радара:").x) / 2);
    ImGui::Text("Координаты радара:");

    ImGui::PushItemWidth(32);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("X").x - 32) / 2);
    ImGui::DragInt("X", &simulationData.x, 1, 0, MAX_X);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Y").x - 32) / 2);
    ImGui::DragInt("Y", &simulationData.y, 1, 0, MAX_Y);

    ImGui::NewLine();

    // Target type

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() -
                          ImGui::CalcTextSize("Выберите тип цели:Крылатая ракетаИстребительБомбардировщик").x -
                          ImGui::GetStyle().ItemSpacing.x * 3) / 2);
    ImGui::Text("Цель:");

    ImGui::SameLine();
    ImGui::RadioButton("Крылатая ракета", &simulationData.targetType, 2);

    ImGui::SameLine();
    ImGui::RadioButton("Истребитель", &simulationData.targetType, 1);

    ImGui::SameLine();
    ImGui::RadioButton("Бомбардировщик", &simulationData.targetType, 0);

    ImGui::NewLine();

    // Target height
    ImGui::SetCursorPosX(
            (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Высота цели:М").x - 32 - ImGui::GetStyle().ItemSpacing.x) /
            2);
    ImGui::Text("Высота цели:");

    ImGui::SameLine();
    ImGui::PushItemWidth(32);
    ImGui::DragFloat("м", &simulationData.heightValue, 1.0, 0, 1000, "%.0f");

    ImGui::NewLine();

    // Height map

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 192) / 2);
    if (ImGui::Button("Выберите файл высот", ImVec2(192, 32))) {
        simulationData.filePath = openFile();
    }

    ImGui::NewLine();

    // Simulation start

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 192) / 2);
    if (ImGui::Button("Начать симуляцию", ImVec2(192, 32))) {
        simulationData.isCorrect = false;
        errorMessage = "";
        isError = false;

        if (simulationData.filePath.empty()) {
            isError = true;
            errorMessage.append("ВЫБЕРИТЕ ФАЙЛ!\n");
        }
        if (simulationData.x < 0 || simulationData.y > 249 || simulationData.x > 249 || simulationData.y < 0) {
            isError = true;
            errorMessage.append("НЕКОРРЕКТНЫЕ ДАННЫЕ! X,Y {0, 249}\n");
        }
        if (simulationData.heightValue < 0 || simulationData.heightValue > 1000) {
            isError = true;
            errorMessage.append("НЕКОРРЕКТНЫЕ ДАННЫЕ! Высота {0, 1000}");
        }
        if (!isError) {
            simulationData.isCorrect = true;
            isRunning = false;
        }
    }


    if (isError) {
        ImGui::NewLine();
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(errorMessage.c_str()).x) / 2);
        ImGui::Text("%s", errorMessage.c_str());
    }

    ImGui::End();
}

std::string GuiWindow::openFile() {
    OPENFILENAME fileStruct;
    ZeroMemory(&fileStruct, sizeof(fileStruct));
    fileStruct.lStructSize = sizeof(OPENFILENAME); // Size of the structure itself
    fileStruct.hwndOwner = NULL; // Owner? Can be null
    fileStruct.lpstrFile = szFile; // Filename in selection field
    fileStruct.lpstrFile[0] = '\0'; //  The first character of this buffer must be NULL if initialization is not necessary
    fileStruct.nMaxFile = sizeof(szFile); // Max filename+path
    fileStruct.lpstrFilter = NULL;
    fileStruct.lpstrFilter = "All\0*.*\0Text\0*.TXT\0"; // Filter by types
    /* Limit file type
    fileStruct.lpstrFilter = "All\0*.*\0Text\0*.TXT\0"; // Filter by types
    fileStruct.nFilterIndex = 1; // Current filter number
     */
    fileStruct.lpstrFileTitle = NULL; // What file exactly should we search
    fileStruct.nMaxFileTitle = 0; // Size of the previous file
    fileStruct.lpstrInitialDir = NULL; // Start directory
    fileStruct.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    GetOpenFileNameA(&fileStruct);


    return fileStruct.lpstrFile;
}

void GuiWindow::drawMainMenu() {
    if (!isRunning) return;

    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({(float) GuiWindow::width, (float) GuiWindow::height});
    ImGui::Begin(
            windowName.c_str(),
            &isRunning,
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoCollapse
    );


    ImGui::PushFont(bigFont);
    ImGui::SetCursorPosY((ImGui::GetWindowHeight() - ImGui::CalcTextSize("МЕНЮ").y - 160) / 2);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("МЕНЮ").x) / 2);
    ImGui::Text("МЕНЮ");
    ImGui::PopFont();

    ImGui::SetCursorPosY((ImGui::GetWindowHeight() - 40) / 2);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 256) / 2);
    if (ImGui::Button("Старт", ImVec2(256, 40))) {
        isMainMenu = false;
    }

    ImGui::SetCursorPosY((ImGui::GetWindowHeight() + 40 + ImGui::GetStyle().ItemSpacing.y) / 2);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 256) / 2);
    if (ImGui::Button("Выход", ImVec2(256, 40))) {
        isRunning = false;
        simulationData.close = true;
    }

    ImGui::End();
}

SimulationData GuiWindow::getSimulationData() {
    simulationData.heightValue /= 1000;
    if (simulationData.x <= 0) {
        simulationData.x = 1;
    }
    if (simulationData.y <= 0) {
        simulationData.y = 1;
    }
    simulationData.y = 249 - simulationData.y;
    return simulationData;
}
