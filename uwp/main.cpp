#include <Windows.h>

#include "SDL2/SDL.h"
#include "../NewSonicThing/src/engineTester/main.hpp"

int SDL_main(int argc, char* argv[])
{
    // This project is an uncommon mashup of SDL and glfw, using the SDL main wrapper gets us a lot of handy UWP event handling for free
    return SAB2::ExternalEntry(0, 0);
}

// Entry point into app (Note, SDL doesn't like being init from here you must call SDL_main)
int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR argv, int argc)
{
    return SDL_WinRTRunApp(SDL_main, NULL);
}
