#include <iostream>
#include "graphics.h"
#include "characters.h"
#include "sprite.h"
#include "scrollingbackground.h"
#include "minimap.h"

using namespace std;

int main(int argc, char *argv[]) {
    Graphics graphics;
    graphics.init();

    ScrollingBackground background;
    background.setTexture(graphics.loadTexture(BACKGROUND_IMG));

    Characters character;
    character.init(graphics);

    SpawnEnemies enemy;
   // enemy.init();

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        bool isMoving = false;
        int deltaX = 0, deltaY = 0;

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT  || character.health <= 0) quit = true;
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                cerr << "Left mouse button pressed\n";
                if (character.type == WARRIOR) character.warriorSlash(graphics);
                else character.archerShoot(graphics);
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.scancode == SDL_SCANCODE_F) character.boost();
                else if (e.key.keysym.scancode == SDL_SCANCODE_1) character.switchToWarrior();
                else if (e.key.keysym.scancode == SDL_SCANCODE_2) character.switchToArcher();
            }
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_LEFT]) {
            deltaX += character.speed;
            isMoving = true;
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            deltaX -= character.speed;
            isMoving = true;
        }
        if (keys[SDL_SCANCODE_UP]) {
            deltaY += character.speed;
            isMoving = true;
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            deltaY -= character.speed;
            isMoving = true;
        }
        graphics.prepareScene();

        background.scroll(deltaX, deltaY);

        enemy.spawn(graphics);

        enemy.update(character, background, graphics);

        if (character.type == WARRIOR) {
            character.warriorMove(isMoving);
            character.updateSlash();
        } else {
            character.archerMove(isMoving);
            character.updateShoot(graphics);
            character.updateArrows();
        }

        character.updateBoost();

        background.render(graphics);

        if (character.type == WARRIOR) {
            if (character.slash.isActing) character.slash.render(character.X, character.Y, graphics);
            else character.move1.render(character.X, character.Y, graphics);
        }
        else {
            if (character.shoot.isActing) character.shoot.render(character.X, character.Y, graphics);
            else character.move2.render(character.X, character.Y, graphics);
            character.renderArrows(graphics);
        }

        enemy.render(graphics);
        renderMiniMap(graphics, character, enemy, background);
        graphics.presentScene();
    }

    background.quit();
    character.quit();
    enemy.quit();
    graphics.quit();

    return 0;
}
