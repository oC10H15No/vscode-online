#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>
#include <algorithm>
#include <set>
#include <array>
#include <unordered_map>
#include <random>
#include <limits>

using namespace std;

// board information
const int BOARD_SIZE = 12; // 棋盘尺寸
#define INF 2147483647

// other information
const int MAX_DEPTH = 4; // 搜索最大深度

// command information
#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"

// 点的状态
enum State
{
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2,
};

int myFlag;
int enemyFlag;

const int BOARD_MIDDLE_1 = (BOARD_SIZE + 1) / 2 - 1; // 5
const int BOARD_MIDDLE_2 = BOARD_SIZE / 2;           // 6

struct coordinate
{
    int x;
    int y;
};

struct Point
{
    coordinate pos;
    int val;
};

struct Nude
{
    int board[BOARD_SIZE][BOARD_SIZE] = {{0}};
    int state = 0;
    Nude()
    {
        board[BOARD_MIDDLE_1][BOARD_MIDDLE_1] = WHITE;
        board[BOARD_MIDDLE_2][BOARD_MIDDLE_2] = WHITE;
        board[BOARD_MIDDLE_2][BOARD_MIDDLE_1] = BLACK;
        board[BOARD_MIDDLE_1][BOARD_MIDDLE_2] = BLACK;
    }
    void arr_input(int x, int y, int playerround)
    {
        board[x][y] = playerround;
    }
} B;

bool judgeInRange(coordinate temp)
{ // 判断该位置是否在棋盘范围内
    return temp.x >= 0 && temp.x < BOARD_SIZE && temp.y >= 0 && temp.y < BOARD_SIZE;
}
int getColor(coordinate target)
{ // 获取该位置的棋子种类
    return B.board[target.y][target.x];
}
void place(coordinate target, int flag)
{ // 放置棋子
    B.board[target.y][target.x] = flag;
}

class gomokuAI
{
public:
    // 五子棋游戏的主 Alpha-Beta 剪枝搜索入口
    coordinate action(Nude)
    {
        // 递归搜索找到最佳落子位置
        Point bestPoint = alphaBeta(B.board, -INF, INF, true, MAX_DEPTH);
        return bestPoint.pos;
    }

private:
    // 获取棋盘上所有可能落子的位置（空气节点）
    vector<coordinate> getRoundCells(int board[BOARD_SIZE][BOARD_SIZE])
    {
        vector<coordinate> emptyCells;
        set<pair<int, int>> locSet;

        for (int y = 0; y < BOARD_SIZE; y++)
        {
            for (int x = 0; x < BOARD_SIZE; x++)
            {
                if (board[y][x] != EMPTY)
                { // 非空，周围一圈内空的加进来
                    for (int dx = -1; dx <= 1; dx++)
                    {
                        for (int dy = -1; dy <= 1; dy++)
                        {
                            int nx = x + dx, ny = y + dy;
                            if (dx == 0 && dy == 0)
                                continue; // 忽略当前位置
                            if (judgeInRange({nx, ny}) && board[ny][nx] == EMPTY)
                            {
                                locSet.insert({nx, ny});
                            }
                        }
                    }
                }
            }
        }
        for (set<pair<int, int>>::iterator it = locSet.begin(); it != locSet.end(); it++)
        {
            emptyCells.push_back({it->first, it->second});
        }
        return emptyCells;
    }

    // Alpha-Beta 剪枝算法
    Point alphaBeta(int board[BOARD_SIZE][BOARD_SIZE], int alpha, int beta, bool maximizingPlayer, int depth)
    {
        std::vector<coordinate> emptyCells = getRoundCells(board);

        if (isGameOver(board) || emptyCells.empty())
        {
            int score = evaluate(board, maximizingPlayer);
            return {score, -1, -1};
        }

        if (depth == 0)
        {
            int score = evaluate(board, maximizingPlayer);
            return {score, -1, -1};
        }

        Point bestPoint = {maximizingPlayer ? -INF : INF, -1, -1};

        if (maximizingPlayer)
        {
            for (size_t i = 0; i < emptyCells.size(); ++i)
            {
                const coordinate &cell = emptyCells[i];
                board[cell.y][cell.x] = myFlag;
                Point point = alphaBeta(board, alpha, beta, false, depth - 1);
                board[cell.y][cell.x] = EMPTY;

                if (point.val > bestPoint.val)
                {
                    bestPoint = point;
                    bestPoint.pos = cell;
                }
                alpha = max(alpha, bestPoint.val);
                if (beta <= alpha)
                    break;
            }
        }
        else
        {
            for (size_t i = 0; i < emptyCells.size(); ++i)
            {
                const coordinate &cell = emptyCells[i];
                board[cell.y][cell.x] = enemyFlag;
                Point point = alphaBeta(board, alpha, beta, true, depth - 1);
                board[cell.y][cell.x] = EMPTY;

                if (point.val < bestPoint.val)
                {
                    bestPoint = point;
                    bestPoint.pos = cell;
                }
                beta = min(beta, bestPoint.val);
                if (beta <= alpha)
                    break;
            }
        }

        return bestPoint;
    }

    // 判断是否满足胜利条件
    bool isGameOver(int board[BOARD_SIZE][BOARD_SIZE])
    {
        const int rows = BOARD_SIZE;
        const int cols = BOARD_SIZE;

        // 检查水平方向
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols - 4; col++)
            {
                if (
                    board[row][col] != 0 &&
                    board[row][col] == board[row][col + 1] &&
                    board[row][col] == board[row][col + 2] &&
                    board[row][col] == board[row][col + 3] &&
                    board[row][col] == board[row][col + 4])
                {
                    return true; // 游戏结束
                }
            }
        }

        // 检查垂直方向
        for (int col = 0; col < cols; col++)
        {
            for (int row = 0; row < rows - 4; row++)
            {
                if (
                    board[row][col] != 0 &&
                    board[row][col] == board[row + 1][col] &&
                    board[row][col] == board[row + 2][col] &&
                    board[row][col] == board[row + 3][col] &&
                    board[row][col] == board[row + 4][col])
                {
                    return true; // 游戏结束
                }
            }
        }

        // 检查正斜对角线方向
        for (int row = 0; row < rows - 4; row++)
        {
            for (int col = 0; col < cols - 4; col++)
            {
                if (
                    board[row][col] != 0 &&
                    board[row][col] == board[row + 1][col + 1] &&
                    board[row][col] == board[row + 2][col + 2] &&
                    board[row][col] == board[row + 3][col + 3] &&
                    board[row][col] == board[row + 4][col + 4])
                {
                    return true; // 游戏结束
                }
            }
        }

        // 检查反斜对角线方向
        for (int row = 4; row < rows; row++)
        {
            for (int col = 0; col < cols - 4; col++)
            {
                if (
                    board[row][col] != 0 &&
                    board[row][col] == board[row - 1][col + 1] &&
                    board[row][col] == board[row - 2][col + 2] &&
                    board[row][col] == board[row - 3][col + 3] &&
                    board[row][col] == board[row - 4][col + 4])
                {
                    return true; // 游戏结束
                }
            }
        }
        return false;
    }

    // 评估函数，计算当前局面得分
    int evaluate(int board[BOARD_SIZE][BOARD_SIZE], bool isMyTurn)
    {
        if (isWinning(board, myFlag))
        {
            return INF;
        }
        else if (isWinning(board, enemyFlag))
        {
            return -INF;
        }
        else
        {
            return evaluateNormal(board, isMyTurn);
        }
    }

    int evaluateNormal(int board[BOARD_SIZE][BOARD_SIZE], bool isMy)
    {
        int score;

        // 遍历横向条
        for (int y = 0; y < BOARD_SIZE; y++)
        {
            std::vector<int> row(board[y], board[y] + BOARD_SIZE);
            score += evaluateLine(row, isMy);
        }

        // 遍历竖向条
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            std::vector<int> line;
            for (int y = 0; y < BOARD_SIZE; y++)
            {
                line.push_back(board[y][x]);
            }
            score += evaluateLine(line, isMy);
        }

        // 遍历正斜向条
        for (int d = 0; d <= BOARD_SIZE + BOARD_SIZE - 2; d++)
        {
            std::vector<int> line;
            for (int x = std::max(0, d - BOARD_SIZE + 1); x <= std::min(d, BOARD_SIZE - 1); x++)
            {
                int y = d - x;
                line.push_back(board[y][x]);
            }
            score += evaluateLine(line, isMy);
        }

        // 遍历反斜向条
        for (int d = 0; d <= BOARD_SIZE + BOARD_SIZE - 2; d++)
        {
            std::vector<int> line;
            for (int x = std::max(0, d - BOARD_SIZE + 1); x <= std::min(d, BOARD_SIZE - 1); x++)
            {
                int y = BOARD_SIZE - 1 - (d - x);
                line.push_back(board[y][x]);
            }
            score += evaluateLine(line, isMy);
        }

        return score;
    }

    // 评估指定位置的得分
    std::unordered_map<std::string, std::array<int, 2>> &getScoreTable()
    {
        static std::unordered_map<std::string, std::array<int, 2>> scoreTable;
        if (!scoreTable.empty())
            return scoreTable;

        int p0 = 100000000;
        int p1 = 100000;
        int p2 = 1000;
        int p3 = 10;
        int p4 = 1;
        int d4 = p1;
        int d3 = p2;
        int d2 = p3;
        int d1 = p4;

        std::unordered_map<std::string, std::array<int, 2>> res{
            {"11111", {p0 + p0, p0}},
            {"_1111_", {p1 + d4, d4}},
            {"_111_1_", {p1 + d3 + d1, d3 + d1}},
            {"_11_11_", {p1 + d2 * 2, d2 * 2}},
            {"21111_", {p1 + 0, 0}},
            {"2111_1_", {p1 + d1, d1}},
            {"21_1112", {p1 + 0, 0}},
            {"211_112", {p1 + 0, 0}},
            {"__111__", {p2 + d3, d3}},
            {"_1_11_", {p2 + d2 + d1, d2 + d1}},
            {"_1__11_", {p2 + d2 + d1, d2 + d1}},
            {"2_111__", {p2 + 0, 0}},
            {"2111__", {p2 + 0, 0}},
            {"21_11_", {p2 + 0, 0}},
            {"21__11_", {p2 + d2, d2}},
            {"2__1112", {p2 + 0, 0}},
            {"2_111_2", {p2 + 0, 0}},
            {"21_11_2", {p2 + 0, 0}},
            {"21_1_12", {p2 + 0, 0}},
            {"_1_1_1_", {p2 + d2 + d1, d2 + d1}},
            {"211_1__", {p2 + 0, 0}},
            {"211_1_2", {p2 + 0, 0}},
            {"__11__", {p3 + d2, d2}},
            {"_1_1_", {p3 + 2 * d1, 2 * d1}},
            {"211___", {p3 + 0, 0}},
            {"21_1__", {p3 + 0, 0}},
            {"21__1_", {p3 + 0, 0}},
            {"__1__", {p4 + d1, d1}}};

        for (std::unordered_map<std::string, std::array<int, 2>>::iterator it = res.begin(); it != res.end(); ++it)
        {
            std::string reversedKey = std::string(it->first.rbegin(), it->first.rend());
            scoreTable[reversedKey] = it->second;
        }

        scoreTable.insert(res.begin(), res.end());
        return scoreTable;
    }

    int evaluateLine(const std::vector<int> &line, bool isMy)
    {
        if (line.size() < 5)
            return 0;

        std::string myLineStr, opLineStr;
        for (size_t i = 0; i < line.size(); ++i)
        {
            int num = line[i];
            if (num == EMPTY)
            {
                myLineStr += '_';
                opLineStr += '_';
            }
            else if (num == enemyFlag)
            {
                myLineStr += '2';
                opLineStr += '1';
            }
            else if (num == myFlag)
            {
                myLineStr += '1';
                opLineStr += '2';
            }
        }

        int score = 0;
        const std::unordered_map<std::string, std::array<int, 2>> &scoreTable = getScoreTable();
        for (std::unordered_map<std::string, std::array<int, 2>>::const_iterator it = scoreTable.begin(); it != scoreTable.end(); ++it)
        {
            const std::string &pattern = it->first;
            const std::array<int, 2> &scores = it->second;
            if (myLineStr.find(pattern) != std::string::npos)
            {
                score += isMy ? scores[0] : scores[1];
            }
            if (opLineStr.find(pattern) != std::string::npos)
            {
                score -= isMy ? scores[1] : scores[0];
            }
        }
        return score;
    }

    bool isWinning(int board[BOARD_SIZE][BOARD_SIZE], int symbol)
    {
        const int target = 5;


        // 水平方向检查
        for (int row = 0; row < BOARD_SIZE; row++)
        {
            int count = 0;
            for (int col = 0; col < BOARD_SIZE; col++)
            {
                count = (board[row][col] == symbol) ? count + 1 : 0;
                if (count == target)
                    return true;
            }
        }

        // 垂直方向检查
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            int count = 0;
            for (int row = 0; row < BOARD_SIZE; row++)
            {
                count = (board[row][col] == symbol) ? count + 1 : 0;
                if (count == target)
                    return true;
            }
        }

        // 对角线方向检查
        const int diagonalOffsets[2][2] = {{1, 1}, {-1, 1}};
        for (int k = 0; k < 2; k++)
        {
            int rowOffset = diagonalOffsets[k][0];
            int colOffset = diagonalOffsets[k][1];
            for (int row = 0; row <= BOARD_SIZE - target; row++)
            {
                for (int col = 0; col <= BOARD_SIZE - target; col++)
                {
                    int count = 0;
                    for (int i = 0; i < target; i++)
                    {
                        int newRow = row + i * rowOffset;
                        int newCol = col + i * colOffset;
                        if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE && board[newRow][newCol] == symbol)
                        {
                            count++;
                            if (count == target)
                                return true;
                        }
                        else
                        {
                            count = 0;
                            break;
                        }
                    }
                }
            }
        }

        return false;
    }

} AI;
bool firstStep = false;
void loop()
{
    char tag[10] = {0};
    int status;
    coordinate command;
    coordinate current;
    while (1)
    {
        memset(tag, 0, sizeof(tag));
        scanf("%s", tag);
        if (strcmp(tag, START) == 0)
        {
            scanf("%d", &myFlag);
            enemyFlag = 3 - myFlag;
            if (myFlag == 1)
            { // 黑棋
                firstStep = true;
                current.x = BOARD_MIDDLE_1;
                current.y = BOARD_MIDDLE_2;
            }
            else
            { // 白棋
                current.x = BOARD_MIDDLE_1;
                current.y = BOARD_MIDDLE_1;
            }
            printf("OK\n");
            fflush(stdout);
        }
        else if (strcmp(tag, PLACE) == 0)
        {
            scanf("%d %d", &command.x, &command.y);
            B.board[command.y][command.x] = enemyFlag;
        }
        else if (strcmp(tag, TURN) == 0)
        {
            if (firstStep)
            {
                firstStep = false;
                printf("%d %d\n", 4, 4);
                place({4, 4}, myFlag);
                fflush(stdout);
            }
            else
            {
                coordinate bestPoint = AI.action(B);
                if (judgeInRange(bestPoint) && getColor(bestPoint) == EMPTY)
                {
                    printf("%d %d\n", bestPoint.x, bestPoint.y);
                    fflush(stdout);
                    place(bestPoint, myFlag);
                }
            }
        }
        else if (strcmp(tag, END) == 0)
        {
            scanf("%d", &status);
            exit(0);
        }
    }
}

int main()
{
    loop();
    return 0;
}