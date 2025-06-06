#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "defs.h"

struct Graphics {
    SDL_Renderer *renderer;
    SDL_Window *window;

    void logErrorAndExit(const char* msg, const char* error);

    void init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
            logErrorAndExit("SDL_Init", SDL_GetError());

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit("SDL_image error:", IMG_GetError());

        if (Mix_Init(MIX_INIT_MP3) < 0)
            logErrorAndExit("Mix_Init", Mix_GetError());
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
            logErrorAndExit("Mix_OpenAudio", Mix_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

        if (TTF_Init() == -1) logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ",TTF_GetError());

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    void prepareScene(SDL_Texture *background = nullptr) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        if (background != nullptr) SDL_RenderCopy(renderer, background, NULL, NULL);
    }

    void presentScene() {
        SDL_RenderPresent(renderer);
    }

    SDL_Texture *loadTexture(const char *filename) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL) {
            logErrorAndExit("Load texture", IMG_GetError());
        }
        return texture;
    }

    void renderTexture(SDL_Texture *texture, int x, int y) {
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y) {
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;
        dest.w = src->w;
        dest.h = src->h;
        SDL_RenderCopy(renderer, texture, src, &dest);
    }

    Mix_Music *loadMusic(const char* path);

    void playMusic(Mix_Music *gMusic);

    Mix_Chunk* loadSound(const char* path);

    void playSound(Mix_Chunk* gChunk);

    TTF_Font* loadFont(const char* path, int size);

    SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color textColor);

    void quit();
};

#endif // _GRAPHICS__H
