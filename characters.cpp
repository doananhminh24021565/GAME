#include "characters.h"

void Characters::init(Graphics &graphics){
    slash.COOLDOWN_DURATION = 500;
    shoot.COOLDOWN_DURATION = 500;
    slash.FRAME_DURATION = 10;
    shoot.FRAME_DURATION = 50;

    SDL_Texture* move1Texture = graphics.loadTexture(WARRIOR_MOVE_FILE);
    SDL_Texture* move2Texture = graphics.loadTexture(ARCHER_MOVE_FILE);
    SDL_Texture* slashTexture = graphics.loadTexture(WARRIOR_SLASH_FILE);
    SDL_Texture* shootTexture = graphics.loadTexture(ARCHER_SHOOT_FILE);
    arrowTexture = graphics.loadTexture(ARROW_FILE);

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
    if (slashSound && slash.isActing) graphics.playSound(slashSound);
}

void Characters::archerShoot(Graphics& graphics){
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - arrowCooldownStartTime >= ARROW_COOLDOWN_DURATION) {
        shoot.act();
        SDL_GetMouseState(&mouseX, &mouseY);
        if (shootSound) graphics.playSound(shootSound);
    }
}

void Characters::updateShoot(Graphics &graphics){
    bool wasActing = shoot.isActing;
    shoot.updateAct();
    if (wasActing && !shoot.isActing) {
        Arrow arrow;
        float distance = 21.0;
        float centerX = X + characterCenterX;
        float centerY = Y + characterCenterY;
        float angleRad = atan2(mouseY - centerY, mouseX - centerX);
        float distanceX = distance * cos(angleRad);
        float distanceY = distance * sin(angleRad);
        float startX = centerX + distanceX;
        float startY = centerY + distanceY;
        arrow.init(graphics, startX, startY, mouseX, mouseY);
        arrows.push_back(std::move(arrow));
        arrowCooldownStartTime = SDL_GetTicks();
        if (arrowSound) graphics.playSound(arrowSound);
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
