#pragma once
#include "SDL.h"
#include <vector>
#include <utility>

enum Square
{
    EMPTY, WHITE, BLACK
};

class Board
{
public:
    Board();
    ~Board();

    Square getWinner();
    void switchPlayers();
    Square getCurrentPlayer();
    bool isOpenSpot(int colIndex, int rowIndex);

    void putPiece(int colIndex, int rowIndex);
    bool attemptAdd(int mouseX, int mouseY);
    void addPiece(int mouseX, int mouseY);

    void renderBoard(SDL_Renderer* renderer);

    int checkNextSpace(int colIndex, int rowIndex, Square color, int colDelta, int rowDelta, int count);
    bool checkBothDirections(int colIndex, int rowIndex, Square color, int colDelta, int rowDelta);
    Square informedWinState(int colIndex, int rowIndex, Square color);

    // 蒙特卡洛模拟函数：返回 {黑胜局数, 白胜局数}
    std::pair<int,int> simulateGames(int simulations);

    // 收集所有空位坐标
    std::vector<std::pair<int,int>> collectEmptyPositions();

    // Monte Carlo 选点：在每个空位先下白子，再模拟，返回最优落点
    std::pair<int,int> findBestMove(int simulations);

private:
    Square currentPlayer = BLACK;
    Square boardState[15][15] = { EMPTY };
    Square winner = EMPTY;
};
