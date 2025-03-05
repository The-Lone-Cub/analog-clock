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

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "Analog Clock",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    // Create clock and renderer instances
    Clock clock;
    Renderer clockRenderer(800, 600);
    if (!clockRenderer.init()) {
        std::cout << "Clock renderer could not be initialized! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Clean up the initial window and renderer as we'll use the ones from clockRenderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    bool quit = false;
    SDL_Event e;

    // Main loop
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Update clock every second
        Uint32 currentTime = SDL_GetTicks();
        static Uint32 lastTime = 0;
        if (currentTime - lastTime >= 1000) {
            clock.update();
            lastTime = currentTime;
        }

        // Render clock
        clockRenderer.render(clock);

        // Cap to 60 FPS
        SDL_Delay(16);
    }

    // Add cleanup before return
    clockRenderer.cleanup();

    return 0;
}
