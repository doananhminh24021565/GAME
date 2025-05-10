#ifndef _CHARACTERS__H
#define _CHARACTERS__H

#include "arrow.h"
#include "sprite.h"

enum CharacterType { WARRIOR, ARCHER };

struct Characters {
    Sprite move1, move2, slash, shoot;
    std::vector<Arrow> arrows;
    SDL_Texture* arrowTexture;
    Mix_Chunk* slashSound = nullptr;
    Mix_Chunk* shootSound = nullptr;
    Mix_Chunk* arrowSound = nullptr;
    const Uint32 BOOST_DURATION = 2000; // 2000ms
    const Uint32 BOOST_COOLDOWN_DURATION = 5000;
    Uint32 ARROW_COOLDOWN_DURATION = 200; // Giảm từ 500ms xuống 200ms
    Uint32 boostStartTime = 0;
    Uint32 arrowCooldownStartTime = 0;
    Uint32 boostCooldownStartTime = 0;
    int X = 300, Y = 200;
    int speed = 2;
    int health = 10;
    bool isBoosting = false;
    CharacterType type = WARRIOR;
    int mouseX, mouseY;

    void init(Graphics& graphics);

    void warriorMove(bool isMoving) { move1.tick(isMoving); }

    void archerMove(bool isMoving) { move2.tick(isMoving); }

    void warriorSlash(Graphics& graphics);

    void updateSlash() { slash.updateAct(); }

    void archerShoot(Graphics& graphics);

    void updateShoot(Graphics &graphics);

    void updateArrows() {
        for (auto it = arrows.begin(); it != arrows.end();) {
            it->update();
            if (!it->isActive) {
                it = arrows.erase(it);
            } else {
                ++it;
            }
        }
    }

    void renderArrows(Graphics& graphics) {
        for (auto& arrow : arrows) {
            arrow.render(graphics);
        }
    }

    void boost();

    void updateBoost();

    void switchToArcher();

    void switchToWarrior();

    void quit(){
        move1.quit();
        move2.quit();
        slash.quit();
        shoot.quit();
        SDL_DestroyTexture(arrowTexture);
        arrowTexture = nullptr;
        for (auto& arrow : arrows) {
            arrow.quit();
        }
        if (slashSound) Mix_FreeChunk(slashSound);
        if (shootSound) Mix_FreeChunk(shootSound);
        if (arrowSound) Mix_FreeChunk(arrowSound);
        slashSound = nullptr;
        shootSound = nullptr;
        arrowSound = nullptr;
    }
};

#endif
