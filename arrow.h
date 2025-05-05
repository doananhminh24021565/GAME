#ifndef _ARROW__H
#define _ARROW__H

#include "graphics.h"
#include <cmath>
#include <iostream>

struct Arrow {
    float x, y; // Tọa độ tâm của mũi tên
    float vx, vy;
    SDL_Texture* texture;
    double angleDeg;
    bool isActive;
    const int SPEED = 10;
    const int ARROW_WIDTH = 200; // Kích thước arrow.png
    const int ARROW_HEIGHT = 200;
    const int ARROW_CENTER_X = 76; // Đuôi của arrow
    const int ARROW_CENTER_Y = 96;

    Arrow() : x(0), y(0), vx(0), vy(0), texture(nullptr), angleDeg(0), isActive(false) {}

    Arrow(Arrow&& other) noexcept
        : x(other.x), y(other.y), vx(other.vx), vy(other.vy),
          texture(other.texture), angleDeg(other.angleDeg), isActive(other.isActive) {
        other.texture = nullptr;
    }

    Arrow& operator=(Arrow&& other) noexcept {
        if (this != &other) {
            if (texture) {
                SDL_DestroyTexture(texture);
            }
            x = other.x;
            y = other.y;
            vx = other.vx;
            vy = other.vy;
            texture = other.texture;
            angleDeg = other.angleDeg;
            isActive = other.isActive;
            other.texture = nullptr;
        }
        return *this;
    }

    Arrow(const Arrow&) = delete;
    Arrow& operator=(const Arrow&) = delete;

    void init(Graphics &graphics, float startX, float startY, float targetX, float targetY) {
        texture = graphics.loadTexture(ARROW_FILE);
        if (!texture) {
            std::cerr << "Failed to load arrow texture: " << IMG_GetError() << "\n";
        }
        x = startX;
        y = startY;
        isActive = true;
        float dx = targetX - startX;
        float dy = targetY - startY;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance > 0.1f) {
            vx = (dx / distance) * SPEED;
            vy = (dy / distance) * SPEED;
            angleDeg = std::atan2(dy, dx) * (180.0 / M_PI);
        } else {
            vx = SPEED;
            vy = 0;
            angleDeg = 0;
        }
        if (x < -ARROW_WIDTH || x > SCREEN_WIDTH || y < -ARROW_HEIGHT || y > SCREEN_HEIGHT) {
            isActive = false;
            std::cerr << "Arrow initialized outside screen: (" << x << ", " << y << ")\n";
        }
        std::cerr << "Arrow initialized: start=(" << x << ", " << y << "), target=(" << targetX << ", " << targetY << "), vx=" << vx << ", vy=" << vy << "\n";
    }

    void update() {
        x += vx;
        y += vy;
        std::cerr << "Arrow updated at (" << x << ", " << y << "), vx=" << vx << ", vy=" << vy << ", isActive=" << isActive << "\n";
        if (x < -ARROW_WIDTH || x > SCREEN_WIDTH || y < -ARROW_HEIGHT || y > SCREEN_HEIGHT) {
            isActive = false;
            std::cerr << "Arrow deactivated (out of bounds)\n";
        }
    }

    void render(Graphics& graphics) {
        if (!isActive) {
            std::cerr << "Arrow not rendered: isActive=false\n";
            return;
        }
        if (!texture) {
            std::cerr << "Arrow render failed: texture is null\n";
            return;
        }
        SDL_Rect dest = { static_cast<int>(x - ARROW_CENTER_X), static_cast<int>(y - ARROW_CENTER_Y), ARROW_WIDTH, ARROW_HEIGHT };
        SDL_Point center = { ARROW_CENTER_X, ARROW_CENTER_Y };
        SDL_RenderCopyEx(graphics.renderer, texture, nullptr, &dest, angleDeg, &center, SDL_FLIP_NONE);
        std::cerr << "Arrow rendered at center=(" << x << ", " << y << "), top-left=(" << dest.x << ", " << dest.y << ")\n";
    }

    void quit() {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }
};

#endif
