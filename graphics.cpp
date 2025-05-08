#include "graphics.h"

void Graphics::logErrorAndExit(const char* msg, const char* error) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        IMG_Quit();
        Mix_CloseAudio();
        Mix_Quit();
        TTF_Quit();
        SDL_Quit();
        exit(1);
}

Mix_Music* Graphics::loadMusic(const char* path) {
    Mix_Music *gMusic = Mix_LoadMUS(path);
    if (gMusic == nullptr) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Could not load music! SDL_mixer Error: %s", Mix_GetError());
    return gMusic;
}

void Graphics::playMusic(Mix_Music *gMusic) {
    if (gMusic == nullptr) return;
    if (Mix_PlayingMusic() == 0) Mix_PlayMusic( gMusic, -1 );
    else if( Mix_PausedMusic() == 1 ) Mix_ResumeMusic();
}

Mix_Chunk* Graphics::loadSound(const char* path) {
    Mix_Chunk* gChunk = Mix_LoadWAV(path);
    if (gChunk == nullptr) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
    return gChunk;
}

void Graphics::playSound(Mix_Chunk* gChunk) {
    if (gChunk != nullptr) Mix_PlayChannel( -1, gChunk, 0 );
}

void Graphics::quit() {
        IMG_Quit();
        Mix_CloseAudio();
        Mix_Quit();
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
}

TTF_Font* Graphics::loadFont(const char* path, int size) {
    TTF_Font* gFont = TTF_OpenFont( path, size );
    if (gFont == nullptr) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load font %s", TTF_GetError());
    return gFont;
}

SDL_Texture* Graphics::renderText(const char* text, TTF_Font* font, SDL_Color textColor) {
    SDL_Surface* textSurface = TTF_RenderText_Solid( font, text, textColor );
    if( textSurface == nullptr ) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Render text surface %s", TTF_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, textSurface );
    if( texture == nullptr ) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Create texture from text %s", SDL_GetError());
    }
    SDL_FreeSurface( textSurface );
    return texture;
}

