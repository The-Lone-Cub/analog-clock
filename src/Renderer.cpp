#include "Renderer.h"
#include <cmath>

Renderer::Renderer(int width, int height) : window(nullptr), renderer(nullptr), width(width), height(height) {}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    window = SDL_CreateWindow("Analog Clock", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        return false;
    }

    return true;
}

void Renderer::cleanup() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void Renderer::render(const Clock& clock) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    drawClockFace();
    drawHourMarkers();

    // Draw hour hand
    drawHand(clock.getHourAngle(), height * 0.2f, 8);
    
    // Draw minute hand
    drawHand(clock.getMinuteAngle(), height * 0.3f, 4);
    
    // Draw second hand
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    drawHand(clock.getSecondAngle(), height * 0.35f, 2);

    SDL_RenderPresent(renderer);
}

void Renderer::drawClockFace() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    int centerX = width / 2;
    int centerY = height / 2;
    int radius = std::min(width, height) / 2 - 20;

    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        int x = centerX + static_cast<int>(radius * cos(angle));
        int y = centerY + static_cast<int>(radius * sin(angle));
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

void Renderer::drawHourMarkers() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    int centerX = width / 2;
    int centerY = height / 2;
    int radius = std::min(width, height) / 2 - 20;

    for (int hour = 0; hour < 12; hour++) {
        float angle = hour * 30 * M_PI / 180.0f;
        int x1 = centerX + static_cast<int>((radius - 10) * cos(angle));
        int y1 = centerY + static_cast<int>((radius - 10) * sin(angle));
        int x2 = centerX + static_cast<int>(radius * cos(angle));
        int y2 = centerY + static_cast<int>(radius * sin(angle));
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

void Renderer::drawHand(float angle, float length, int thickness) {
    int centerX = width / 2;
    int centerY = height / 2;
    float radians = angle * M_PI / 180.0f;

    int targetX = centerX + static_cast<int>(length * cos(radians));
    int targetY = centerY + static_cast<int>(length * sin(radians));

    for (int i = -thickness/2; i <= thickness/2; i++) {
        for (int j = -thickness/2; j <= thickness/2; j++) {
            SDL_RenderDrawLine(renderer, centerX + i, centerY + j, targetX + i, targetY + j);
        }
    }
}