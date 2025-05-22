#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

#undef main

Game::Game()
{
    board = new Board;
}

Game::~Game()
{
    delete board;
}

void Game::init(const char* title, int xPos, int yPos, int w, int h, bool fullscreen)
{
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        this->width = w;
        this->height = h;
        window = SDL_CreateWindow(title, xPos, yPos, this->width, this->height, flags);
        renderer = SDL_CreateRenderer(window, -1, 0);
        board->renderBoard(renderer);
        isRunning = true;
        srand(static_cast<unsigned>(time(NULL)));
    }
    else isRunning = false;
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT)
        isRunning = false;
    else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        board->attemptAdd(event.motion.x, event.motion.y);
    }
}

void Game::update()
{
    // 游戏结束直接退出
    if (board->getWinner() != EMPTY)
    {
        stopGame();
        return;
    }

    // 如果轮到白方（AI）且还没开始思考
    if (board->getCurrentPlayer() == WHITE && !aiThinking)
    {
        std::cout << "白棋 AI 正在思考..." << std::endl;
        aiThinking  = true;
        aiStartTime = SDL_GetTicks();
        return;  // 本帧先不落子
    }

    // 思考延迟结束后真正落子
    if (board->getCurrentPlayer() == WHITE && aiThinking)
    {
        if (SDL_GetTicks() - aiStartTime >= static_cast<Uint32>(aiDelay))
        {
            auto [ax, ay] = board->findBestMove(200);
            if (ax >= 0)
            {
                board->putPiece(ax, ay);
                std::cout << "白方(AI) 落子: (" << ax << "," << ay << ")" << std::endl;

                if (board->informedWinState(ax, ay, WHITE) == WHITE)
                {
                    std::cout << "白方(AI) 胜利!" << std::endl;
                    stopGame();
                }
                else
                {
                    board->switchPlayers();
                }
            }
            aiThinking = false;
        }
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 128,90,70,255);
    board->renderBoard(renderer);
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Game::stopGame()
{
    isRunning = false;
}
