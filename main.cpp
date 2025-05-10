#include <iostream>
#include "graphics.h"
#include "sprite.h"
#include "scrollingbackground.h"
#include "function.h"
#include "minimap.h"
#include "main_menu.h"

using namespace std;

// Định nghĩa trạng thái trò chơi

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

    // Khởi tạo font để hiển thị thông tin
    TTF_Font* font = graphics.loadFont(FONT_FILE, 14);
    SDL_Color textColor = {255, 255, 255, 255}; // Màu trắng

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (character.health <= 0) gameState = MENU, character.health = 10;
            if (gameState == MENU && menu.handleEvent(e, quit)) gameState = PLAYING; // Chuyển sang trạng thái chơi
            else if (gameState == PLAYING) handleEvent(e, character, graphics, gameState);
        }

        graphics.prepareScene();

        if (gameState == MENU) {
            menu.render(graphics);
        } else if (gameState == PLAYING) {
            bool isMoving = false;
            std::pair<int,int> delta = getDelta(character.speed, isMoving);
            background.scroll(delta.first, delta.second);
            enemy.spawn(graphics);
            enemy.update(character, background, graphics);
            updateActing(character, graphics, isMoving);
            background.render(graphics);
            Acting(character, graphics);
            enemy.render(graphics);
            renderMiniMap(graphics, character, enemy, background);
            printText(character, graphics, font, textColor);
        }

        graphics.presentScene();
    }

    // Giải phóng tài nguyên
    TTF_CloseFont(font);
    menu.quit();
    background.quit();
    character.quit();
    enemy.quit();
    graphics.quit();

    return 0;
}
