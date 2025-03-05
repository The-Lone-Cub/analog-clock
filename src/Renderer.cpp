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

    // Draw hour hand (green)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    drawHand(clock.getHourAngle(), height * 0.2f, 3);
    
    // Draw minute hand (blue)
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    drawHand(clock.getMinuteAngle(), height * 0.3f, 3);
    
    // Draw second hand (red)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    drawHand(clock.getSecondAngle(), height * 0.35f, 3);

    SDL_RenderPresent(renderer);
}

void Renderer::drawClockFace() {
    // Draw the solid clock body
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    int centerX = width / 2;
    int centerY = height / 2;
    int radius = std::min(width, height) / 2 - 20;

    // Fill the clock body
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }

    // Draw the clock outline
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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

    // Draw hour markers (thicker)
    for (int hour = 0; hour < 12; hour++) {
        float angle = hour * 30 * M_PI / 180.0f;
        float cos_angle = cos(angle);
        float sin_angle = sin(angle);
        
        // Calculate perpendicular vector for thickness
        float perpX = -sin_angle;
        float perpY = cos_angle;
        
        for (int offset = -2; offset <= 2; offset++) {
            float offsetX = perpX * offset;
            float offsetY = perpY * offset;
            
            int x1 = centerX + static_cast<int>((radius - 15) * cos_angle + offsetX);
            int y1 = centerY + static_cast<int>((radius - 15) * sin_angle + offsetY);
            int x2 = centerX + static_cast<int>(radius * cos_angle + offsetX);
            int y2 = centerY + static_cast<int>(radius * sin_angle + offsetY);
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    }

    // Draw minute markers (shorter)
    for (int minute = 0; minute < 60; minute++) {
        if (minute % 5 != 0) {  // Skip positions where hour markers are
            float angle = minute * 6 * M_PI / 180.0f;
            int x1 = centerX + static_cast<int>((radius - 8) * cos(angle));
            int y1 = centerY + static_cast<int>((radius - 8) * sin(angle));
            int x2 = centerX + static_cast<int>(radius * cos(angle));
            int y2 = centerY + static_cast<int>(radius * sin(angle));
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    }
}

void Renderer::drawHand(float angle, float length, int thickness) {
    int centerX = width / 2;
    int centerY = height / 2;
    float radians = angle * M_PI / 180.0f;

    float targetX = centerX + length * cos(radians);
    float targetY = centerY + length * sin(radians);

    // Draw the main line with anti-aliasing
    float dx = targetX - centerX;
    float dy = targetY - centerY;
    float len = sqrt(dx * dx + dy * dy);
    
    if (len < 1) return;

    dx /= len;
    dy /= len;

    // Draw anti-aliased thick line
    for (float offset = -(thickness-1)/2.0f; offset <= thickness/2.0f; offset += 1.0f) {
        float perpX = -dy * offset;
        float perpY = dx * offset;

        float x = centerX + perpX;
        float y = centerY + perpY;
        float endX = targetX + perpX;
        float endY = targetY + perpY;

        // Draw anti-aliased line
        float gradient = dy / dx;

        bool steep = fabs(dy) > fabs(dx);
        if (steep) {
            std::swap(x, y);
            std::swap(endX, endY);
            gradient = 1 / gradient;
        }

        if (x > endX) {
            std::swap(x, endX);
            std::swap(y, endY);
        }

        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

        for (float px = x; px <= endX; px += 1.0f) {
            if (steep) {
                float progress = (px - x) / (endX - x);
                float py = y + (endY - y) * progress;
                int ipx = static_cast<int>(py);
                float fpart = py - ipx;
                Uint8 c1 = static_cast<Uint8>((1.0f - fpart) * 255);
                Uint8 c2 = static_cast<Uint8>(fpart * 255);
                SDL_SetRenderDrawColor(renderer, r, g, b, c1);
                SDL_RenderDrawPoint(renderer, ipx, static_cast<int>(px));
                SDL_SetRenderDrawColor(renderer, r, g, b, c2);
                SDL_RenderDrawPoint(renderer, ipx + 1, static_cast<int>(px));
            } else {
                float progress = (px - x) / (endX - x);
                float py = y + (endY - y) * progress;
                int ipx = static_cast<int>(py);
                float fpart = py - ipx;
                Uint8 c1 = static_cast<Uint8>((1.0f - fpart) * 255);
                Uint8 c2 = static_cast<Uint8>(fpart * 255);
                SDL_SetRenderDrawColor(renderer, r, g, b, c1);
                SDL_RenderDrawPoint(renderer, static_cast<int>(px), ipx);
                SDL_SetRenderDrawColor(renderer, r, g, b, c2);
                SDL_RenderDrawPoint(renderer, static_cast<int>(px), ipx + 1);
            }
        }
    }
}