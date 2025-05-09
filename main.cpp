#include <iostream>
#include "graphics.h"
#include "characters.h"
#include "sprite.h"
#include "scrollingbackground.h"
#include "minimap.h"
#include "main_menu.h"

using namespace std;

// Định nghĩa trạng thái trò chơi
enum GameState { MENU, PLAYING };

int main(int argc, char *argv[]) {
    Graphics graphics;
    graphics.init();

    // Khởi tạo menu
    MainMenu menu;
    menu.init(graphics, FONT_FILE, 48); // Thay bằng đường dẫn font thực tế

    // Khởi tạo âm nhạc
    Mix_Music *gMusic = graphics.loadMusic(BACKGROUND_MUSIC);
    graphics.playMusic(gMusic);

    // Khởi tạo các đối tượng trò chơi
    ScrollingBackground background;
    background.setTexture(graphics.loadTexture(BACKGROUND_IMG));

    Characters character;
    character.init(graphics);

    SpawnEnemies enemy;

    bool quit = false;
    SDL_Event e;
    GameState gameState = MENU; // Bắt đầu ở trạng thái menu

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (gameState == MENU) {
                if (menu.handleEvent(e, quit)) gameState = PLAYING; // Chuyển sang trạng thái chơi
            }
            else if (gameState == PLAYING) {
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) gameState = MENU; // Quay lại menu khi nhấn Esc
                    else if (e.key.keysym.scancode == SDL_SCANCODE_F) character.boost();
                    else if (e.key.keysym.scancode == SDL_SCANCODE_1) character.switchToWarrior();
                    else if (e.key.keysym.scancode == SDL_SCANCODE_2) character.switchToArcher();
                }
                else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    cerr << "Left mouse button pressed\n";
                    if (character.type == WARRIOR) character.warriorSlash(graphics);
                    else character.archerShoot(graphics);
                }
            }
        }

        graphics.prepareScene();

        if (gameState == MENU) {
            menu.render(graphics);
        }
        else if (gameState == PLAYING) {
            bool isMoving = false;
            int deltaX = 0, deltaY = 0;

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
        }

        graphics.presentScene();
    }

    // Giải phóng tài nguyên
    menu.quit();
    background.quit();
    character.quit();
    enemy.quit();
    graphics.quit();

    return 0;
}
