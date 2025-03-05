#include <SDL2/SDL.h>
#include <iostream>
#include "clock.h"
#include "renderer.h"

int main(int, char**) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create clock and renderer instances
    Clock clock;
    Renderer clockRenderer(800, 600);
    if (!clockRenderer.init()) {
        std::cout << "Clock renderer could not be initialized! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    
    bool quit = false;
    SDL_Event e;
    
    // Frame timing variables
    const int TARGET_FPS = 30;  // Reduced from 60 to 30 FPS
    const int FRAME_DELAY = 1000 / TARGET_FPS;
    const int UPDATE_INTERVAL = 1000;  // Update clock every second
    Uint32 frameStart;
    int frameTime;
    bool needsRedraw = true;
    
    // Main loop
    while (!quit) {
        // Handle all pending events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                break;
            }
        }
        
        if (quit) break;
        
        frameStart = SDL_GetTicks();
        
        // Update clock every second
        Uint32 currentTime = SDL_GetTicks();
        static Uint32 lastTime = 0;
        if (currentTime - lastTime >= UPDATE_INTERVAL) {
            clock.update();
            lastTime = currentTime;
            needsRedraw = true;
        }

        // Only render when needed
        if (needsRedraw) {
            clockRenderer.render(clock);
            needsRedraw = false;
        }

        // Calculate frame time and delay
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    // Cleanup
    clockRenderer.cleanup();
    SDL_Quit();
    return 0;
}
