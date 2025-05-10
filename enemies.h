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
    Sprite explosionSprite; // Sprite cho hiệu ứng nổ
    Mix_Chunk* explosionSound = nullptr;
    bool isActive;
    bool isExploding; // Trạng thái đang phát hiệu ứng nổ
    int lastOffsetX, lastOffsetY;
    Uint32 explosionStartTime; // Thời điểm bắt đầu nổ
    static const Uint32 EXPLOSION_DURATION = 500; // Thời gian nổ (ms)

    void init(Graphics& graphics, float startX, float startY) {
        X = startX;
        Y = startY;

        // Sinh tốc độ ngẫu nhiên trong khoảng [1, 10]
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> speedDist(1.0f, 10.0f);
        speed = speedDist(gen);

        health = 100;
        isActive = true;
        isExploding = false;
        lastOffsetX = 0;
        lastOffsetY = 0;
        explosionStartTime = 0;

        // Khởi tạo sprite enemy
        SDL_Texture* texture = graphics.loadTexture(ENEMY_FILE);
        if (!texture) {
            std::cerr << "Failed to load enemy texture: " << IMG_GetError() << "\n";
            throw std::runtime_error("Texture loading failed");
        }
        sprite.init(texture, MOVE_CLIPS, _frameDelay, sizeof(MOVE_CLIPS) / sizeof(int) / 4);

        // Khởi tạo sprite nổ
        SDL_Texture* explosionTexture = graphics.loadTexture("boom.png");
        if (!explosionTexture) {
            std::cerr << "Failed to load explosion texture: " << IMG_GetError() << "\n";
            throw std::runtime_error("Explosion texture loading failed");
        }
        explosionSprite.init(explosionTexture, BOOM_CLIPS, _explosionFrameDelay, sizeof(BOOM_CLIPS) / sizeof(int) / 4);

        explosionSound = graphics.loadSound(EXPLOSION_SOUND);
    }

    void update(float targetX, float targetY, ScrollingBackground& background) {
        if (!isActive && !isExploding) return;

        if (isExploding) {
            // Cập nhật animation nổ
            explosionSprite.tick(true);
            if (SDL_GetTicks() - explosionStartTime >= EXPLOSION_DURATION) {
                isExploding = false; // Kết thúc hiệu ứng nổ
                isActive = false; // Đánh dấu để xóa
            }
            return;
        }

        // Logic di chuyển bình thường
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
            // Hiển thị sprite nổ
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
