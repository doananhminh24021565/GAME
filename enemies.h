#ifndef _ENEMIES__H
#define _ENEMIES__H

#include "sprite.h"
#include "scrollingbackground.h"
#include "defs.h"
#include <random>

struct Enemy {
    float X, Y;
    float speed;
    int health;
    Sprite sprite;
    Sprite explosionSprite;
    Mix_Chunk* explosionSound = nullptr;
    bool isActive;
    bool isExploding;
    int lastOffsetX, lastOffsetY;
    Uint32 explosionStartTime;
    static const Uint32 EXPLOSION_DURATION = 500;

    void init(Graphics& graphics, float startX, float startY) {
        X = startX;
        Y = startY;
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> speedDist(1.0f, 3.0f);
        speed = speedDist(gen);
        health = 100;
        isActive = true;
        isExploding = false;
        lastOffsetX = 0;
        lastOffsetY = 0;
        explosionStartTime = 0;
        SDL_Texture* texture = graphics.loadTexture(ENEMY_FILE);
        sprite.init(texture, MOVE_CLIPS, _frameDelay, sizeof(MOVE_CLIPS) / sizeof(int) / 4);
        SDL_Texture* explosionTexture = graphics.loadTexture(BOOM_FILE);
        explosionSprite.init(explosionTexture, BOOM_CLIPS, _explosionFrameDelay, sizeof(BOOM_CLIPS) / sizeof(int) / 4);
        explosionSound = graphics.loadSound(EXPLOSION_SOUND);
    }

    void update(float targetX, float targetY, ScrollingBackground& background) {
        if (!isActive && !isExploding) return;
        if (isExploding) {
            explosionSprite.tick(true);
            if (SDL_GetTicks() - explosionStartTime >= EXPLOSION_DURATION) {
                isExploding = false;
                isActive = false;
            }
            return;
        }
        float centerX = X + characterCenterX;
        float centerY = Y + characterCenterY;
        float dx = targetX - centerX;
        float dy = targetY - centerY;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance > 0) {
            dx /= distance;
            dy /= distance;
            X += dx * speed;
            Y += dy * speed;
        }

        int deltaOffsetX = background.scrollingOffsetX - lastOffsetX;
        int deltaOffsetY = background.scrollingOffsetY - lastOffsetY;

        X += deltaOffsetX;
        Y += deltaOffsetY;

        lastOffsetX = background.scrollingOffsetX;
        lastOffsetY = background.scrollingOffsetY;

        sprite.tick(true);
    }

    void render(Graphics& graphic, int targetX, int targetY) {
        if (isExploding) {
            const SDL_Rect* clip = explosionSprite.getCurrentClip();
            SDL_Rect renderQuad = {X + 50, Y + 50, clip->w, clip->h};
            SDL_RenderCopy(graphic.renderer, explosionSprite.texture, clip, &renderQuad);
        }
        else if (isActive) {
            const SDL_Rect* clip = sprite.getCurrentClip();
            SDL_Rect renderQuad = {X, Y, clip->w, clip->h};
            float centerX = X + characterCenterX;
            float centerY = Y + characterCenterY;
            float angleRad = atan2(targetY - centerY, targetX - centerX);
            float angleDeg = angleRad * (180.0 / M_PI);
            SDL_Point center = {characterCenterX, characterCenterY};
            SDL_RenderCopyEx(graphic.renderer, sprite.texture, clip, &renderQuad, angleDeg, &center, SDL_FLIP_NONE);
        }
    }

    void quit() {
        sprite.quit();
        explosionSprite.quit();
    }
};

#endif
