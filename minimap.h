#ifndef _MINIMAP__H
#define _MINIMAP__H

#include "characters.h"
#include "spawnenemies.h"

void renderMiniMap(Graphics &graphic, Characters& character, SpawnEnemies& enemy, ScrollingBackground& background){
        const int minimapWidth = 100;
        const int minimapHeight = 75;
        SDL_SetRenderDrawColor(graphic.renderer, 0, 255, 0, 0.5); // Nền xanh
        SDL_Rect minimapRect = { 0, 0, minimapWidth, minimapHeight };
        SDL_RenderFillRect(graphic.renderer, &minimapRect);

        // Tính tỷ lệ thu nhỏ
        float scaleX = (float)minimapWidth / background.width;
        float scaleY = (float)minimapHeight / background.height;

        // Vẽ chấm trắng cho nhân vật
        int charPosX = (character.X - background.scrollingOffsetX) * scaleX;
        int charPosY = (character.Y - background.scrollingOffsetY) * scaleY;

        std::cout << background.scrollingOffsetX << ' ' << background.scrollingOffsetY << '\n';
        SDL_SetRenderDrawColor(graphic.renderer, 255, 255, 255, 255); // Trắng
        SDL_Rect charRect = { charPosX - 2, charPosY - 2, 4, 4 }; // Chấm 4x4 pixel
        SDL_RenderFillRect(graphic.renderer, &charRect);

        // Vẽ chấm đỏ cho kẻ thù
        SDL_SetRenderDrawColor(graphic.renderer, 255, 0, 0, 255); // Đỏ
        for (auto& it: enemy.enemies) {
            if (it.isActive) {
                int enemyPosX = (it.X - background.scrollingOffsetX) * scaleX;
                int enemyPosY = (it.Y - background.scrollingOffsetY) * scaleY;
                SDL_Rect enemyRect = { enemyPosX - 2, enemyPosY - 2, 4, 4 }; // Chấm 4x4 pixel
                SDL_RenderFillRect(graphic.renderer, &enemyRect);
            }
        }
}

#endif
