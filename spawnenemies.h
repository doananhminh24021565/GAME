#ifndef _SPAWNENEMIES__H
#define _SPAWNENEMIES__H

#include "enemies.h"
#include "characters.h"
#include "scrollingbackground.h"
#include <random>
#include <cmath>

struct SpawnEnemies {
    std::vector<Enemy> enemies;
    Uint32 enemySpawnTime = 0;
    Uint32 ENEMY_SPAWN_INTERVAL_MIN = 1000;
    Uint32 ENEMY_SPAWN_INTERVAL_MAX = 3000;
    const size_t MAX_ENEMIES = 10;
    const float COLLISION_RADIUS = 50.0f;
    const float SWORD_RADIUS = 100.0f;

    void spawn(Graphics& graphics) {
        if (enemies.size() >= MAX_ENEMIES) return;

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - enemySpawnTime < ENEMY_SPAWN_INTERVAL_MIN) return;

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> intervalDist(ENEMY_SPAWN_INTERVAL_MIN, ENEMY_SPAWN_INTERVAL_MAX);
        enemySpawnTime = currentTime + intervalDist(gen);

        std::uniform_int_distribution<> xDist(0, 6400);
        std::uniform_int_distribution<> yDist(0, 3000);
        float startX = static_cast<float>(xDist(gen));
        float startY = static_cast<float>(yDist(gen));

        Enemy enemy;
        enemy.init(graphics, startX, startY);
        enemies.push_back(std::move(enemy));
        std::cerr << "Enemy spawned at (" << startX << ", " << startY << ")\n";
    }

    void update(Characters& character, ScrollingBackground& background, Graphics& graphics) {
        float targetCenterX = character.X + characterCenterX;
        float targetCenterY = character.Y + characterCenterY;
        for (auto it = enemies.begin(); it != enemies.end();) {
            it->update(targetCenterX, targetCenterY, background);

            if (it->isActive && !it->isExploding) {
                float enemyCenterX = it->X + characterCenterX;
                float enemyCenterY = it->Y + characterCenterY;
                float dx = targetCenterX - enemyCenterX;
                float dy = targetCenterY - enemyCenterY;
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance < COLLISION_RADIUS) {
                    it->isExploding = true;
                    it->explosionStartTime = SDL_GetTicks();
                    graphics.playSound(it->explosionSound);
                    if (!character.health) return;
                    else character.health -- ;
                    std::cerr << "Enemy collided with character at (" << it->X << ", " << it->Y << "), exploding, character blinking\n";
                }
                else if (character.type == WARRIOR && character.slash.isActing && distance < SWORD_RADIUS) {
                    it->isExploding = true;
                    it->explosionStartTime = SDL_GetTicks();
                    graphics.playSound(it->explosionSound);
                    std::cerr << "Enemy hit by warrior slash at (" << it->X << ", " << it->Y << "), exploding\n";
                }
                else if (character.type == ARCHER) {
                    for (auto& arrow : character.arrows) {
                        if (arrow.isActive) {
                            float arrowEnemyDx = arrow.x - enemyCenterX;
                            float arrowEnemyDy = arrow.y - enemyCenterY;
                            float arrowDistance = std::sqrt(arrowEnemyDx * arrowEnemyDx + arrowEnemyDy * arrowEnemyDy);
                            if (arrowDistance < COLLISION_RADIUS) {
                                it->isExploding = true;
                                it->explosionStartTime = SDL_GetTicks();
                                graphics.playSound(it->explosionSound);
                                arrow.isActive = false;
                                std::cerr << "Enemy hit by arrow at (" << it->X << ", " << it->Y << "), exploding, arrow deactivated\n";
                                break;
                            }
                        }
                    }
                }
            }

            if (!it->isActive && !it->isExploding) it = enemies.erase(it);
            else ++it;
        }
    }

    void render(Graphics& graphics) {
        for (auto& enemy : enemies) enemy.render(graphics, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    }

    void quit() {
        for (auto& enemy : enemies) {
            enemy.quit();
        }
    }
};

#endif
