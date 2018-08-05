cmake_minimum_required(VERSION 3.7)
project(ASCCI_Space_Destroyer_CLion)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

find_package(SDL)


include_directories(${SDL_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        SDL_GFX/SDL_gfxPrimitives.c
        SDL_GFX/SDL_gfxBlitFunc.c
        SDL_GFX/SDL_rotozoom.c
        Ally.cpp
        Foe.cpp
        Game.cpp
        HitManager.cpp
        Score.cpp
        Shot.cpp
        SpaceShip.cpp
        WaveManager.cpp
        Control/Input.cpp
        Control/InputAndJoy.cpp
        Shot/Brexit.cpp
        Shot/Cutter.cpp
        Shot/Disper.cpp
        Shot/Gun.cpp
        Shot/Hecto.cpp
        Shot/Missile.cpp
        Shot/Part.cpp
        Shot/Photo.cpp
        Shot/Sniper.cpp)

add_executable(ASCCI_Space_Destroyer_CLion ${SOURCE_FILES})

target_link_libraries(ASCCI_Space_Destroyer_CLion ${SDL_LIBRARY})

file(COPY Resources DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}/)