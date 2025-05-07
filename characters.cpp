#include "characters.h"
#include <iostream>

void Characters::init(Graphics &graphics){
    slash.COOLDOWN_DURATION = 500;
    shoot.COOLDOWN_DURATION = 500;
    slash.FRAME_DURATION = 10;
    shoot.FRAME_DURATION = 50; // 50ms mỗi frame, tổng 250ms cho 5 frames

    SDL_Texture* move1Texture = graphics.loadTexture(WARRIOR_MOVE_FILE);
    if (!move1Texture) {
        std::cerr << "Failed to load warrior move texture: " << IMG_GetError() << "\n";
        throw std::runtime_error("Texture loading failed");
    }
    SDL_Texture* move2Texture = graphics.loadTexture(ARCHER_MOVE_FILE);
    if (!move2Texture) {
        std::cerr << "Failed to load archer move texture: " << IMG_GetError() << "\n";
        throw std::runtime_error("Texture loading failed");
    }
    SDL_Texture* slashTexture = graphics.loadTexture(WARRIOR_SLASH_FILE);
    if (!slashTexture) {
        std::cerr << "Failed to load warrior slash texture: " << IMG_GetError() << "\n";
        throw std::runtime_error("Texture loading failed");
    }
    SDL_Texture* shootTexture = graphics.loadTexture(ARCHER_SHOOT_FILE);
    if (!shootTexture) {
        std::cerr << "Failed to load archer shoot texture: " << IMG_GetError() << "\n";
        throw std::runtime_error("Texture loading failed");
    }
    arrowTexture = graphics.loadTexture(ARROW_FILE);
    if (!arrowTexture) {
        std::cerr << "Failed to load arrow texture: " << IMG_GetError() << "\n";
        throw std::runtime_error("Texture loading failed");
    }
    slashSound = graphics.loadSound(SLASH_SOUND);
    shootSound = graphics.loadSound(SHOOT_SOUND);
    arrowSound = graphics.loadSound(ARROW_SOUND);

    move1.init(move1Texture, MOVE_CLIPS, _frameDelay, sizeof(MOVE_CLIPS)/sizeof(int)/4);
    move2.init(move2Texture, MOVE_CLIPS, _frameDelay, sizeof(MOVE_CLIPS)/sizeof(int)/4);
    slash.init(slashTexture, SLASH_CLIPS, _frameDelay, sizeof(SLASH_CLIPS)/sizeof(int)/4);
    shoot.init(shootTexture, SHOOT_CLIPS, _frameDelay, sizeof(SHOOT_CLIPS)/sizeof(int)/4);
}

void Characters::warriorSlash(Graphics& graphics){
    slash.act();
    if (slashSound) graphics.playSound(slashSound);
    std::cerr << "Warrior slash triggered, sound played\n";
}

void Characters::archerShoot(Graphics& graphics){
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - arrowCooldownStartTime >= ARROW_COOLDOWN_DURATION) {
        shoot.act();
        SDL_GetMouseState(&mouseX, &mouseY);
        if (shootSound) graphics.playSound(shootSound);
        std::cerr << "Archer shoot triggered. Mouse coordinates: (" << mouseX << ", " << mouseY << "), isActing: " << shoot.isActing << ", sound played\n";
    } else {
        std::cerr << "Archer shoot skipped due to cooldown. Time since last shot: " << (currentTime - arrowCooldownStartTime) << "ms\n";
    }
}

void Characters::updateShoot(Graphics &graphics){
    bool wasActing = shoot.isActing;
    shoot.updateAct();
    std::cerr << "updateShoot: wasActing=" << wasActing << ", isActing=" << shoot.isActing << "\n";
    if (wasActing && !shoot.isActing) { // Animation vừa kết thúc
        Arrow arrow;
        float distance = 21.0;
        float centerX = X + characterCenterX;
        float centerY = Y + characterCenterY;
        std::pair <double, double> angle = getAngle(centerX, centerY, static_cast<float>(mouseX), static_cast<float>(mouseY));
        float distanceX = distance * cos(angle.first);
        float distanceY = distance * sin(angle.first);
        float startX = centerX + distanceX;
        float startY = centerY + distanceY;
        arrow.init(graphics, startX, startY, mouseX, mouseY);
        arrows.push_back(std::move(arrow));
        arrowCooldownStartTime = SDL_GetTicks();
        if (arrowSound) graphics.playSound(arrowSound);
        std::cerr << "Arrow created at (" << startX << ", " << startY << ") targeting (" << mouseX << ", " << mouseY << "), sound played\n";
    }
}

void Characters::boost(){
    Uint32 currentTime = SDL_GetTicks();
    if (!isBoosting && currentTime - boostCooldownStartTime >= BOOST_COOLDOWN_DURATION) {
        isBoosting = true;
        boostStartTime = SDL_GetTicks();
        speed = speed * 3;
    }
}

void Characters::updateBoost(){
    if (isBoosting) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - boostStartTime >= BOOST_DURATION) {
            boostCooldownStartTime = currentTime;
            isBoosting = false;
            speed = speed / 3;
        }
    }
}

void Characters::switchToArcher(){
    type = ARCHER;
    slash.isActing = false;
    shoot.isActing = false;
    slash.currentFrame = 0;
    shoot.currentFrame = 0;
    arrows.clear();
    arrowCooldownStartTime = 0;
    mouseX = 0;
    mouseY = 0;
}

void Characters::switchToWarrior(){
    type = WARRIOR;
    slash.isActing = false;
    shoot.isActing = false;
    slash.currentFrame = 0;
    shoot.currentFrame = 0;
    arrows.clear();
    arrowCooldownStartTime = 0;
    mouseX = 0;
    mouseY = 0;
}
