#ifndef _SCROLLINGBACKGROUND__H
#define _SCROLLINGBACKGROUND__H

#include "graphics.h"

struct ScrollingBackground {
    SDL_Texture* texture;
    int scrollingOffsetX = -200; // Cuộn theo trục Ox
    int scrollingOffsetY = -500; // Cuộn theo trục Oy
    int width, height;

    void setTexture(SDL_Texture* _texture) {
        texture = _texture;
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        if (width < SCREEN_WIDTH || height < SCREEN_HEIGHT) {
            std::cerr << "Background texture too small: width=" << width << ", height=" << height << "\n";
        }
    }

    void scroll(int deltaX, int deltaY) {
        if(scrollingOffsetX > 125) {
            scrollingOffsetX = 125;
            return;
        }
        if(scrollingOffsetY > 140) {
            scrollingOffsetY = 140;
            return;
        }
        if(scrollingOffsetX < -6040) {
            scrollingOffsetY = -6040;
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
        int texW = width;
        int texH = height;

        // Tính offset đúng trong khoảng [0, texW) và [0, texH)
        int offsetX = scrollingOffsetX % texW;
        int offsetY = scrollingOffsetY % texH;
        if (offsetX > 0) offsetX -= texW;
        if (offsetY > 0) offsetY -= texH;

        // Vẽ các ô nền để phủ kín màn hình
        for (int x = offsetX; x < SCREEN_WIDTH; x += texW) {
            for (int y = offsetY; y < SCREEN_HEIGHT; y += texH) {
                SDL_Rect dest = { x, y, texW, texH };
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
