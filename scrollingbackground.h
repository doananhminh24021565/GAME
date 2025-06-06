#ifndef _SCROLLINGBACKGROUND__H
#define _SCROLLINGBACKGROUND__H

#include "graphics.h"
#include <random>

struct ScrollingBackground {
    SDL_Texture* texture;
    int scrollingOffsetX = -200; // Cuộn theo trục Ox
    int scrollingOffsetY = -500; // Cuộn theo trục Oy
    int width, height;

    void setTexture(SDL_Texture* _texture) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> xDist(-6040, 240);
        std::uniform_int_distribution<> yDist(-2740, 140);
        scrollingOffsetX = xDist(gen);
        scrollingOffsetY = yDist(gen);
        texture = _texture;
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        if (width < SCREEN_WIDTH || height < SCREEN_HEIGHT) {
            std::cerr << "Background texture too small: width=" << width << ", height=" << height << "\n";
        }
    }

    void scroll(int deltaX, int deltaY) {
        if(scrollingOffsetX > 240) {
            scrollingOffsetX = 240;
            return;
        }
        if(scrollingOffsetY > 140) {
            scrollingOffsetY = 140;
            return;
        }
        if(scrollingOffsetX < -6040) {
            scrollingOffsetX = -6040;
            return;
        }
        if(scrollingOffsetY < -2740) {
            scrollingOffsetY = -2740;
            return;
        }
        scrollingOffsetX += deltaX;
        scrollingOffsetY += deltaY;
    }

    void render(Graphics &graphic) {
        int offsetX = scrollingOffsetX % width;
        int offsetY = scrollingOffsetY % height;
        if (offsetX > 0) offsetX -= width;
        if (offsetY > 0) offsetY -= height;

        for (int x = offsetX; x < SCREEN_WIDTH; x += width) {
            for (int y = offsetY; y < SCREEN_HEIGHT; y += height) {
                SDL_Rect dest = { x, y, width, height };
                SDL_RenderCopy(graphic.renderer, texture, NULL, &dest);
            }
        }
    }

    void quit(){
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
};

#endif
