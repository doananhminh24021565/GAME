#include <iostream>
#include "graphics.h"
#include "sprite.h"
#include "scrollingbackground.h"
#include "function.h"
#include "minimap.h"
#include "main_menu.h"

using namespace std;

int main(int argc, char *argv[]) {
    Graphics graphics;
    graphics.init();

    MainMenu menu;
    menu.init(graphics, FONT_FILE, 48); // Thay bằng đường dẫn font thực tế

    Mix_Music *gMusic = graphics.loadMusic(BACKGROUND_MUSIC);
    graphics.playMusic(gMusic);

    ScrollingBackground background;
    background.setTexture(graphics.loadTexture(BACKGROUND_IMG));

    Characters character;
    character.init(graphics);

    SpawnEnemies enemy;

    bool quit = false;
    SDL_Event e;
    GameState gameState = MENU; // Bắt đầu ở trạng thái menu

    TTF_Font* font = graphics.loadFont(FONT_FILE, 13);
    TTF_Font* gameOverFont = graphics.loadFont(FONT_FILE, 72); // Font lớn hơn cho Game Over
    SDL_Color textColor = {255, 255, 255, 255}; // Màu trắng
    SDL_Color gameOverColor = {255, 0, 0, 255}; // Màu đỏ cho Game Over

    Uint32 gameOverStartTime = 0;
    bool showGameOver = false;
    const Uint32 GAME_OVER_DURATION = 5000; // 5 giây

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (gameState == MENU && menu.handleEvent(e, quit)) gameState = PLAYING; // Chuyển sang trạng thái chơi
            else if (gameState == PLAYING) handleEvent(e, character, graphics, gameState);
        }
        graphics.prepareScene();
        if (gameState == MENU) {
            menu.render(graphics);
        }
        else if (gameState == PLAYING) {
            if (character.health > 0) {
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
            else{
                if (!showGameOver) {
                    showGameOver = true;
                    gameOverStartTime = SDL_GetTicks();
                }
                else {
                    SDL_Texture* gameOverTexture = graphics.renderText("Game Over", gameOverFont, gameOverColor);
                    graphics.renderTexture(gameOverTexture, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 50);
                    if (SDL_GetTicks() - gameOverStartTime >= GAME_OVER_DURATION) {
                        SDL_DestroyTexture(gameOverTexture);
                        showGameOver = false;
                        character.health = 10;
                        gameState = MENU;
                    }
                }
            }
        }

        graphics.presentScene();
    }

    TTF_CloseFont(font);
    TTF_CloseFont(gameOverFont);
    menu.quit();
    background.quit();
    character.quit();
    enemy.quit();
    graphics.quit();

    return 0;
}
