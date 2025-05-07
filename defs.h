#ifndef _DEFS__H
#define _DEFS__H

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int _frameDelay = 80; // ms per frame
const int _explosionFrameDelay = 20;
const int characterCenterX = 100;
const int characterCenterY = 100;

const int SLASH_CLIPS[][4] = {
    {   0, 0, 200, 200}, { 200, 0, 200, 200}, { 400, 0, 200, 200}, { 600, 0, 200, 200},
    { 800, 0, 200, 200}, {1000, 0, 200, 200}, {1200, 0, 200, 200}, {1400, 0, 200, 200},
    {1600, 0, 200, 200}, {1800, 0, 200, 200}, {2000, 0, 200, 200}, {2200, 0, 200, 200},
    {2400, 0, 200, 200}, {2600, 0, 200, 200}, {2800, 0, 200, 200}, {3000, 0, 200, 200},
    {3200, 0, 200, 200}, {3400, 0, 200, 200}, {3600, 0, 200, 200}, {3800, 0, 200, 200},
    {4000, 0, 200, 200},

};

const int MOVE_CLIPS[][4] = {
    {  0, 0, 200, 200},
    {200, 0, 200, 200},
    {400, 0, 200, 200},
    {600, 0, 200, 200},
    {800, 0, 200, 200},
};

const int SHOOT_CLIPS[][4] = {
    {   0, 0, 200, 200},
    { 200, 0, 200, 200},
    { 400, 0, 200, 200},
    { 600, 0, 200, 200},
    { 800, 0, 200, 200},
    {1000, 0, 200, 200},
    {1200, 0, 200, 200},
};

const int BOOM_CLIPS[][4] = {
    {   0, 0, 100, 100},
    { 100, 0, 100, 100},
    { 200, 0, 100, 100},
    { 300, 0, 100, 100},
    { 400, 0, 100, 100},
    { 500, 0, 100, 100},
    { 600, 0, 100, 100},
    { 700, 0, 100, 100},
};

extern const char* BACKGROUND_IMG;
extern const char* WINDOW_TITLE;
extern const char* WARRIOR_MOVE_FILE;
extern const char* WARRIOR_SLASH_FILE;
extern const char* ARCHER_MOVE_FILE;
extern const char* ARCHER_SHOOT_FILE;
extern const char* ARROW_FILE;
extern const char* ENEMY_FILE;
extern const char* BOOM_FILE;
extern const char* SLASH_SOUND;
extern const char* SHOOT_SOUND;
extern const char* ARROW_SOUND;
extern const char* EXPLOSION_SOUND;

#endif
