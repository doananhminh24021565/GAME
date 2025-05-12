#ifndef _MINIMAP__H
#define _MINIMAP__H

#include "characters.h"
#include "spawnenemies.h"

void renderMiniMap(Graphics &graphic, Characters& character, SpawnEnemies& enemy, ScrollingBackground& background){
        const int minimapWidth = 192;
        const int minimapHeight = 90;
        SDL_SetRenderDrawColor(graphic.renderer, 0, 255, 0, 0.5);
        SDL_Rect minimapRect = { 0, 0, minimapWidth, minimapHeight };
        SDL_RenderFillRect(graphic.renderer, &minimapRect);

        float scaleX = (float)minimapWidth / background.width;
        float scaleY = (float)minimapHeight / background.height;

        float charPosX = (character.X - background.scrollingOffsetX) * scaleX;
        float charPosY = (character.Y - background.scrollingOffsetY) * scaleY;

        SDL_SetRenderDrawColor(graphic.renderer, 255, 255, 255, 255);
        SDL_Rect charRect = { charPosX - 2, charPosY - 2, 4, 4 };
        SDL_RenderFillRect(graphic.renderer, &charRect);

        SDL_SetRenderDrawColor(graphic.renderer, 255, 0, 0, 255);
        for (auto& it: enemy.enemies) {
            if (it.isActive) {
                float enemyPosX = (it.X - background.scrollingOffsetX) * scaleX;
                float enemyPosY = (it.Y - background.scrollingOffsetY) * scaleY;
                SDL_Rect enemyRect = { enemyPosX - 2, enemyPosY - 2, 4, 4 };
                SDL_RenderFillRect(graphic.renderer, &enemyRect);
            }
        }
}

#endif
