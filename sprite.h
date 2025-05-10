#ifndef _SPRITE__H
#define _SPRITE__H

#include "graphics.h"
#include <cmath>
#include <vector>

struct Sprite {
    SDL_Texture* texture;
    std::vector<SDL_Rect> clips;
    int currentFrame = 0;
    Uint32 StartTime = 0;
    Uint32 CooldownStartTime = 0;
    Uint32 COOLDOWN_DURATION;
    Uint32 FRAME_DURATION;
    int frames;
    Uint32 lastUpdate = 0;
    Uint32 frameDelay = 100;
    bool isActing = false;

    void init(SDL_Texture* _texture, const int _clips[][4], Uint32 _frameDelay, int _frames) {
        texture = _texture;
        frameDelay = _frameDelay;
        frames = _frames;
        SDL_Rect clip;
        for (int i = 0; i < frames; i++) {
            clip.x = _clips[i][0];
            clip.y = _clips[i][1];
            clip.w = _clips[i][2];
            clip.h = _clips[i][3];
            clips.push_back(clip);
        }
    }

    void tick(bool isMoving) {
        if (!isMoving) {
            currentFrame = 0;
            return;
        }
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastUpdate >= frameDelay) {
            currentFrame = (currentFrame + 1) % clips.size();
            lastUpdate = currentTime;
        }
    }

    void act(){
        Uint32 currentTime = SDL_GetTicks();
        if (!isActing && currentTime - CooldownStartTime >= COOLDOWN_DURATION) {
            isActing = true;
            StartTime = SDL_GetTicks();
            std::cerr << "Sprite act: isActing set to true\n";
        }
    }

    void updateAct(){
        if (isActing) {
            Uint32 currentTime = SDL_GetTicks();
            int elapsedFrames = (currentTime - StartTime) / FRAME_DURATION;
            currentFrame = elapsedFrames % frames;
            if (elapsedFrames >= frames) {
                isActing = false;
                CooldownStartTime = currentTime;
                currentFrame = 0;
                std::cerr << "Sprite updateAct: isActing set to false\n";
            }
        }
    }

    const SDL_Rect* getCurrentClip() const {
        return &(clips[currentFrame]);
    }

    void render(int x, int y, Graphics &graphic){
        const SDL_Rect* clip = getCurrentClip();
        SDL_Rect renderQuad = {x, y, clip->w, clip->h};
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        float centerX = renderQuad.x + characterCenterX;
        float centerY = renderQuad.y + characterCenterY;
        float angleRad = atan2(mouseY - centerY, mouseX - centerX);
        float angleDeg = angleRad * (180.0 / M_PI);
        SDL_Point center = { characterCenterX, characterCenterY };
        SDL_RenderCopyEx(graphic.renderer, texture, clip, &renderQuad, angleDeg, &center, SDL_FLIP_NONE);
    }

    void quit(){
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
};

#endif
