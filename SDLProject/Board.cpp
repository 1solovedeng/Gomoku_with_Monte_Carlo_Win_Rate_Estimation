#include "Board.h"
#include <iostream>
#include <cstdlib>   // 为 rand() 和 srand()
#include <utility>   // 为 std::pair

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
    int xIndex = mouseX / 50;
    int yIndex = mouseY / 50;

    if (isOpenSpot(xIndex, yIndex))
    {
        addPiece(mouseX, mouseY);

        if (informedWinState(xIndex, yIndex, getCurrentPlayer()) == EMPTY)
        {
            // 切换玩家后进行模拟
            switchPlayers();

            // 模拟500局并输出胜率
            auto results = simulateGames(500);
            int blackWins = results.first;
            int whiteWins = results.second;
            int totalWins = blackWins + whiteWins;
            if (totalWins > 0) {
                int blackPct = static_cast<int>(100.0 * blackWins / totalWins);
                int whitePct = static_cast<int>(100.0 * whiteWins / totalWins);
                std::cout << "这一步后的蒙特卡洛模拟胜率: 黑: "
                          << blackPct << "%, 白: " << whitePct << "%" << std::endl;
            } else {
                std::cout << "这一步后的蒙特卡洛模拟胜率: 黑: 0%, 白: 0%" << std::endl;
            }
        }
        else
        {
            winner = getCurrentPlayer();
        }
        return true;
    }
    return false;
}

void Board::addPiece(int mouseX, int mouseY)
{
    int xIndex = mouseX / 50;
    int yIndex = mouseY / 50;
    putPiece(xIndex, yIndex);
}

void Board::renderBoard(SDL_Renderer* renderer)
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // 绘制棋盘网格
    for (int i = 1; i < 15; i++)
    {
        SDL_RenderDrawLine(renderer, GAME_W/15*i, 0, GAME_W/15*i, GAME_H);
        SDL_RenderDrawLine(renderer, 0, GAME_H/15*i, GAME_W, GAME_H/15*i);
    }

    // 绘制棋子
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            SDL_Rect rect = { GAME_W/15*i + 5, GAME_H/15*j + 5, GAME_W/15 - 10, GAME_H/15 - 10 };
            if (boardState[i][j] == WHITE)
            {
                SDL_SetRenderDrawColor(renderer, 255, 250, 250, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
            else if (boardState[i][j] == BLACK)
            {
                SDL_SetRenderDrawColor(renderer, 27, 30, 35, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

int Board::checkNextSpace(int colIndex, int rowIndex, Square color, int colDelta, int rowDelta, int count)
{
    if (colIndex+colDelta < 0 || colIndex+colDelta > 14 ||
        rowIndex+rowDelta < 0 || rowIndex+rowDelta > 14)
        return count;

    if (boardState[colIndex+colDelta][rowIndex+rowDelta] == color)
    {
        count++;
        if (count >= 5) return count;
        return checkNextSpace(colIndex+colDelta, rowIndex+rowDelta, color, colDelta, rowDelta, count);
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
    if (checkBothDirections(colIndex, rowIndex, color, -1, 0) ||
        checkBothDirections(colIndex, rowIndex, color, 0, 1)  ||
        checkBothDirections(colIndex, rowIndex, color, -1, 1) ||
        checkBothDirections(colIndex, rowIndex, color, 1, 1))
    {
        return color;
    }
    return EMPTY;
}

// 新增：蒙特卡洛模拟逻辑
std::pair<int,int> Board::simulateGames(int simulations)
{
    int blackWins = 0;
    int whiteWins = 0;

    for (int i = 0; i < simulations; ++i)
    {
        Board simBoard = *this;
        if (simBoard.getWinner() != EMPTY)
        {
            if (simBoard.getWinner() == BLACK) blackWins++;
            else whiteWins++;
            continue;
        }

        // 收集空格
        std::vector<std::pair<int,int>> empties;
        for (int x = 0; x < 15; ++x)
            for (int y = 0; y < 15; ++y)
                if (simBoard.isOpenSpot(x, y))
                    empties.emplace_back(x, y);

        int moves = 0;
        while (!empties.empty() && moves < 100 && simBoard.getWinner() == EMPTY)
        {
            int idx = rand() % empties.size();
            int cx = empties[idx].first, cy = empties[idx].second;
            simBoard.putPiece(cx, cy);
            Square win = simBoard.informedWinState(cx, cy, simBoard.getCurrentPlayer());
            if (win != EMPTY)
            {
                if (win == BLACK) blackWins++;
                else whiteWins++;
                break;
            }
            simBoard.switchPlayers();
            empties[idx] = empties.back();
            empties.pop_back();
            moves++;
        }
        // 平局不计入
    }

    return std::make_pair(blackWins, whiteWins);
}
