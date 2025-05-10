#ifndef _MAIN_MENU__H
#define _MAIN_MENU__H

#include "graphics.h"

struct MainMenu {
    TTF_Font* font;
    SDL_Texture* playText;
    SDL_Texture* quitText;
    SDL_Texture* background;
    SDL_Rect playRect;
    SDL_Rect quitRect;

    MainMenu() : font(nullptr), playText(nullptr), quitText(nullptr) {
        playRect = {0, 0, 0, 0};
        quitRect = {0, 0, 0, 0};
    }

    void init(Graphics& graphics, const char* fontPath, int fontSize) {
        background = graphics.loadTexture(MAIN_MENU_BACKGROUND);
        font = graphics.loadFont(fontPath, fontSize);
        if (!font) {
            graphics.logErrorAndExit("Failed to load font", TTF_GetError());
        }

        SDL_Color textColor = {255, 255, 255, 255}; // Màu trắng
        playText = graphics.renderText("Play", font, textColor);
        quitText = graphics.renderText("Quit", font, textColor);
        if (!playText || !quitText) {
            graphics.logErrorAndExit("Failed to render text", SDL_GetError());
        }

        // Căn giữa văn bản trên màn hình
        SDL_QueryTexture(playText, NULL, NULL, &playRect.w, &playRect.h);
        SDL_QueryTexture(quitText, NULL, NULL, &quitRect.w, &quitRect.h);
        playRect.x = (SCREEN_WIDTH - playRect.w) / 2;
        playRect.y = SCREEN_HEIGHT / 2 - 75;
        quitRect.x = (SCREEN_WIDTH - quitRect.w) / 2;
        quitRect.y = SCREEN_HEIGHT / 2 + 50;
    }

    bool handleEvent(SDL_Event& e, bool& quit) {
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            if (mouseX >= playRect.x && mouseX <= playRect.x + playRect.w &&
                mouseY >= playRect.y && mouseY <= playRect.y + playRect.h) {
                return true; // Chuyển sang trạng thái chơi
            }
            if (mouseX >= quitRect.x && mouseX <= quitRect.x + quitRect.w &&
                mouseY >= quitRect.y && mouseY <= quitRect.y + quitRect.h) {
                quit = true; // Thoát chương trình
            }
        }
        return false;
    }

    void render(Graphics& graphics) {
        graphics.renderTexture(background, 0, 0);
        graphics.renderTexture(playText, playRect.x, playRect.y);
        graphics.renderTexture(quitText, quitRect.x, quitRect.y);
    }

    void quit() {
        if (playText) SDL_DestroyTexture(playText);
        if (quitText) SDL_DestroyTexture(quitText);
        if (font) TTF_CloseFont(font);
    }
};

#endif // _MAIN_MENU__H
