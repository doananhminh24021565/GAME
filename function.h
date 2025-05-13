#ifndef FUNCTION_H
#define FUNCTION_H

#include <SDL.h>
#include <cmath>
#include <utility>
#include <iostream>
#include "characters.h"

enum GameState { MENU, PLAYING };

inline std::pair<int,int> getDelta(int speed, bool& isMoving) {
    int deltaX = 0, deltaY = 0;

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_LEFT]) deltaX += speed, isMoving = true;
    if (keys[SDL_SCANCODE_RIGHT]) deltaX -= speed, isMoving = true;
    if (keys[SDL_SCANCODE_UP]) deltaY += speed, isMoving = true;
    if (keys[SDL_SCANCODE_DOWN]) deltaY -= speed, isMoving = true;

    return {deltaX, deltaY};
}

inline void handleEvent(SDL_Event& e, Characters& character, Graphics& graphics, GameState& gameState) {
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) gameState = MENU; // Quay lại menu khi nhấn Esc
        else if (e.key.keysym.scancode == SDL_SCANCODE_F) character.boost();
        else if (e.key.keysym.scancode == SDL_SCANCODE_1) character.switchToWarrior();
        else if (e.key.keysym.scancode == SDL_SCANCODE_2) character.switchToArcher();
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        if (character.type == WARRIOR) character.warriorSlash(graphics);
        else character.archerShoot(graphics);
    }
}

inline void Acting(Characters& character, Graphics& graphics) {
    if (character.type == WARRIOR) {
        if (character.slash.isActing) character.slash.render(character.X, character.Y, graphics);
        else character.move1.render(character.X, character.Y, graphics);
    }
    else {
        if (character.shoot.isActing) character.shoot.render(character.X, character.Y, graphics);
        else character.move2.render(character.X, character.Y, graphics);
        character.renderArrows(graphics);
    }
}

inline void updateActing(Characters& character, Graphics& graphics, bool isMoving){
    if (character.type == WARRIOR) {
        character.warriorMove(isMoving);
        character.updateSlash();
    }
    else {
        character.archerMove(isMoving);
        character.updateShoot(graphics);
        character.updateArrows();
    }
    character.updateBoost();
}

inline void printText(Characters& character, Graphics& graphics, TTF_Font* font, SDL_Color& textColor){
    std::string healthText = "Health: " + std::to_string(character.health);
    SDL_Texture* healthTexture = graphics.renderText(healthText.c_str(), font, textColor);
    graphics.renderTexture(healthTexture, SCREEN_WIDTH - 150, 10);
    SDL_DestroyTexture(healthTexture);

    std::string highestScoreText = "Highest score: " + std::to_string(character.highestScore);
    SDL_Texture* highestScoreTexture = graphics.renderText(highestScoreText.c_str(), font, textColor);
    graphics.renderTexture(highestScoreTexture, SCREEN_WIDTH - 150, 40);
    SDL_DestroyTexture(highestScoreTexture);

    std::string scoreText = "Score: " + std::to_string(character.score);
    SDL_Texture* scoreTexture = graphics.renderText(scoreText.c_str(), font, textColor);
    graphics.renderTexture(scoreTexture, SCREEN_WIDTH - 150, 70);
    SDL_DestroyTexture(scoreTexture);

    Uint32 currentTime = SDL_GetTicks();
    std::string slashStatus = (currentTime - character.slash.StartTime >= character.slash.COOLDOWN_DURATION) ? "Slash: Can use" : "Slash: Cannot use";
    SDL_Texture* slashTexture = graphics.renderText(slashStatus.c_str(), font, textColor);
    graphics.renderTexture(slashTexture, SCREEN_WIDTH - 150, 100);
    SDL_DestroyTexture(slashTexture);

    std::string shootStatus = (currentTime - character.shoot.StartTime >= character.shoot.COOLDOWN_DURATION) ? "Shoot: Can use" : "Shoot: Cannot use";
    SDL_Texture* shootTexture = graphics.renderText(shootStatus.c_str(), font, textColor);
    graphics.renderTexture(shootTexture, SCREEN_WIDTH - 150, 130);
    SDL_DestroyTexture(shootTexture);

    std::string boostStatus = (!character.isBoosting && currentTime - character.boostCooldownStartTime >= character.BOOST_COOLDOWN_DURATION) ? "Boost: Can use" : "Boost: Cannot use";
    SDL_Texture* boostTexture = graphics.renderText(boostStatus.c_str(), font, textColor);
    graphics.renderTexture(boostTexture, SCREEN_WIDTH - 150, 160);
    SDL_DestroyTexture(boostTexture);
}
#endif
