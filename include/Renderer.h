#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Clock.h"

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    bool init();
    void render(const Clock& clock);
    void cleanup();

private:
    void filledPolygon(const SDL_Point *points, int count);
    void createClockFaceTexture();
    void createSignatureTexture();
    void drawHand(float angle, float length, int thickness, Uint8 r, Uint8 g, Uint8 b);

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *clockFaceTexture;
    SDL_Texture *signatureTexture;
    TTF_Font *font;
    int width, height, radius;
};

#endif // RENDERER_H
