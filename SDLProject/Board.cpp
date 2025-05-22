#include "Board.h"
#include <iostream>
#include <cstdlib>   // rand, srand
#include <ctime>     // time
#include <limits>

int GAME_W = 750;
int GAME_H = 750;

Board::Board() { }
Board::~Board() { }

Square Board::getWinner()
{
    return winner;
}

void Board::switchPlayers()
{
    currentPlayer = (currentPlayer == WHITE ? BLACK : WHITE);
}

Square Board::getCurrentPlayer()
{
    return currentPlayer;
}

bool Board::isOpenSpot(int colIndex, int rowIndex)
{
    return boardState[colIndex][rowIndex] == EMPTY;
}

void Board::putPiece(int colIndex, int rowIndex)
{
    if (colIndex < 0 || colIndex >= 15) return;
    if (rowIndex < 0 || rowIndex >= 15) return;
    boardState[colIndex][rowIndex] = getCurrentPlayer();
}

bool Board::attemptAdd(int mouseX, int mouseY)
{
    int x = mouseX / 50;
    int y = mouseY / 50;
    if (!isOpenSpot(x, y)) return false;

    putPiece(x, y);

    // 检查是否即时胜出
    if (informedWinState(x, y, getCurrentPlayer()) == EMPTY)
    {
        // 切换到白方
        switchPlayers();

        // 黑方刚下完后输出黑白胜率
        auto [bWins, wWins] = simulateGames(500);
        int total = bWins + wWins;
        if (total > 0)
        {
            int bPct = static_cast<int>(100.0 * bWins / total);
            int wPct = 100 - bPct;
            std::cout << "这一步后的蒙特卡洛模拟胜率: "
                      << "黑方: " << bPct << "%, 白方 : " << wPct << "%"
                      << std::endl;
        }
    }
    else
    {
        // 当前玩家胜出
        winner = getCurrentPlayer();
    }
    return true;
}

void Board::addPiece(int mouseX, int mouseY)
{
    int x = mouseX / 50;
    int y = mouseY / 50;
    putPiece(x, y);
}

void Board::renderBoard(SDL_Renderer* renderer)
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 1; i < 15; ++i)
    {
        SDL_RenderDrawLine(renderer, GAME_W/15*i, 0, GAME_W/15*i, GAME_H);
        SDL_RenderDrawLine(renderer, 0, GAME_H/15*i, GAME_W, GAME_H/15*i);
    }
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
            SDL_Rect rect{ GAME_W/15*i + 5, GAME_H/15*j + 5, GAME_W/15 - 10, GAME_H/15 - 10 };
            if (boardState[i][j] == WHITE)
            {
                SDL_SetRenderDrawColor(renderer, 255,250,250,255);
                SDL_RenderFillRect(renderer, &rect);
            }
            else if (boardState[i][j] == BLACK)
            {
                SDL_SetRenderDrawColor(renderer, 27,30,35,255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

int Board::checkNextSpace(int colIndex, int rowIndex, Square color, int colDelta, int rowDelta, int count)
{
    int cx = colIndex + colDelta, cy = rowIndex + rowDelta;
    if (cx < 0 || cx >= 15 || cy < 0 || cy >= 15) return count;
    if (boardState[cx][cy] == color)
    {
        count++;
        if (count >= 5) return count;
        return checkNextSpace(cx, cy, color, colDelta, rowDelta, count);
    }
    return count;
}

bool Board::checkBothDirections(int colIndex, int rowIndex, Square color, int colDelta, int rowDelta)
{
    return (checkNextSpace(colIndex, rowIndex, color, colDelta, rowDelta, 1)
          + checkNextSpace(colIndex, rowIndex, color, -colDelta, -rowDelta, 0)) >= 5;
}

Square Board::informedWinState(int colIndex, int rowIndex, Square color)
{
    if (checkBothDirections(colIndex, rowIndex, color, 1, 0) ||
        checkBothDirections(colIndex, rowIndex, color, 0, 1) ||
        checkBothDirections(colIndex, rowIndex, color, 1, 1) ||
        checkBothDirections(colIndex, rowIndex, color, 1, -1))
    {
        return color;
    }
    return EMPTY;
}

std::vector<std::pair<int,int>> Board::collectEmptyPositions()
{
    std::vector<std::pair<int,int>> empties;
    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 15; ++j)
            if (boardState[i][j] == EMPTY)
                empties.emplace_back(i, j);
    return empties;
}

std::pair<int,int> Board::simulateGames(int simulations)
{
    int blackWins = 0, whiteWins = 0;
    for (int i = 0; i < simulations; ++i)
    {
        Board sim = *this;
        auto empties = sim.collectEmptyPositions();
        int moves = 0;
        while (!empties.empty() && moves < 100 && sim.getWinner() == EMPTY)
        {
            int idx = rand() % empties.size();
            auto [x, y] = empties[idx];
            sim.putPiece(x, y);
            if (sim.informedWinState(x, y, sim.getCurrentPlayer()) != EMPTY)
            {
                sim.getCurrentPlayer() == BLACK ? ++blackWins : ++whiteWins;
                break;
            }
            sim.switchPlayers();
            empties.erase(empties.begin() + idx);
            ++moves;
        }
    }
    return {blackWins, whiteWins};
}

std::pair<int,int> Board::findBestMove(int simulations)
{
    auto empties = collectEmptyPositions();
    if (empties.empty()) return {-1, -1};

    double bestPct = -1.0;
    std::pair<int,int> bestMove = empties[0];

    for (auto [x, y] : empties)
    {
        Board sim = *this;
        // 白方先下
        sim.putPiece(x, y);
        // 立即胜出
        if (sim.informedWinState(x, y, WHITE) == WHITE)
            return {x, y};

        sim.switchPlayers();
        auto [bWins, wWins] = sim.simulateGames(simulations);
        int total = bWins + wWins;
        double pct = (total > 0) ? double(wWins) / total : 0.0;
        if (pct > bestPct)
        {
            bestPct = pct;
            bestMove = {x, y};
        }
    }

    return bestMove;
}
