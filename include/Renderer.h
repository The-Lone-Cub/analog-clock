#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "Clock.h"

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    bool init();
    void render(const Clock& clock);
    void cleanup();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width;
    int height;

    void drawClockFace();
    void drawHourMarkers();
    void drawHand(float angle, float length, int thickness);
};

#endif // RENDERER_H