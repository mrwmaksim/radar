cmake_minimum_required(VERSION 3.20)
project(radar)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)

# DXSDK_DIR
set(DXSDK_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)")

set(BUILD_SHARED_LIBS 0)

#IMGUI
set(ROOT ${PROJECT_SOURCE_DIR}/ImGui)
set(IMGUI_PUBLIC_HEADERS ${ROOT}/imgui.h)
set(IMGUI_PRIVATE_HEADERS
        ${ROOT}/imgui_internal.h ${ROOT}/imconfig.h ${ROOT}/imstb_rectpack.h
        ${ROOT}/imstb_textedit.h ${ROOT}/imstb_truetype.h)
set(IMGUI_SOURCES ${ROOT}/imgui.cpp ${ROOT}/imgui_draw.cpp
        ${ROOT}/imgui_widgets.cpp ${ROOT}/imgui_demo.cpp)

#PLATFORM
set(PLATFORM_HEADERS ${ROOT}/examples/imgui_impl_dx9.h ${ROOT}/examples/imgui_impl_win32.h)
set(PLATFORM_SOURCE ${ROOT}/examples/imgui_impl_dx9.cpp ${ROOT}/examples/imgui_impl_win32.cpp)

add_library(imgui_static STATIC ${IMGUI_SOURCES} ${IMGUI_PUBLIC_HEADERS}
        ${IMGUI_PRIVATE_HEADERS} ${PLATFORM_HEADERS} ${PLATFORM_SOURCE})


target_include_directories(imgui_static PUBLIC ${ROOT})

if(BUILD_SHARED_LIBS)
    message("Shared")
    add_library(imgui_shared SHARED ${IMGUI_SOURCES})
    target_include_directories(imgui_shared PUBLIC ${ROOT})
    add_library(imgui ALIAS imgui_shared)
else()
    message("No shared")
    add_library(imgui ALIAS imgui_static)
endif()

include_directories(${DXSDK_DIR}/Include)

if (WIN32)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64 bits
        message("Using x64")
        link_directories(${DXSDK_DIR}/Lib/64)
    elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
        # 32 bits
        message("Using x86")
        link_directories(${DXSDK_DIR}/Lib/x86)
    endif ()
endif ()

add_executable(${PROJECT_NAME} include.h LTexture.cpp LTexture.h map.cpp map.h pixel.cpp pixel.h radar.cpp radar.h GuiWindow.h GuiWindow.cpp main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC SDL2-2.0.20/include)
if (WIN32)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64 bits
        message("Using x64")
        target_link_directories(${PROJECT_NAME} PUBLIC SDL2-2.0.20/lib/x64)
    elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
        # 32 bits
        message("Using x86")
        target_link_directories(${PROJECT_NAME} PUBLIC SDL2-2.0.20/lib/x86)
    endif ()
endif ()


target_link_libraries(${PROJECT_NAME} d3d9 imgui SDL2 SDL2_image SDL2_ttf SDL2main SDL2test)


