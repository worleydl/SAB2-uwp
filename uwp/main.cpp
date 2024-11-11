#include <Windows.h>

#include "SDL2/SDL.h"
#include "../NewSonicThing/src/engineTester/main.hpp"

// You can locally declare a SDL_main function or call to a DLL export (mingw works nice for this) 
int SDL_main(int argc, char* argv[])
{
    return SAB2::ExternalEntry(0, 0);
}

// Entry point into app (Note, SDL doesn't like being init from here you must call SDL_main)
int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR argv, int argc)
{
    return SDL_WinRTRunApp(SDL_main, NULL);
}
