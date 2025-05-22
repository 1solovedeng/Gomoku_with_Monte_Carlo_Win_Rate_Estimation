#pragma once
#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "SDL.h"
#include "Board.h"

class Game {
public:
    Game();
    ~Game();

    /**
     * 初始化游戏窗口
     * @param title 窗口标题
     * @param xPos 窗口 X 位置
     * @param yPos 窗口 Y 位置
     * @param w    窗口宽度
     * @param h    窗口高度
     * @param fullscreen 是否全屏
     */
    void init(const char* title, int xPos, int yPos, int w, int h, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() { return isRunning; }
    void stopGame();

private:
    int width, height;
    bool isRunning;
    Board* board;
    SDL_Window* window;
    SDL_Renderer* renderer;

    // —— AI 思考状态相关 ——
    bool aiThinking = false;   // AI 是否正在思考
    Uint32 aiStartTime = 0;    // 开始思考的时间戳
    int aiDelay = 1000;        // 思考延迟（毫秒）
};

#endif // GAME_H
