#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <random>
#include <algorithm>
#include <unordered_map>

enum Status
{
    Empty = 0, // 空位标志
    Black = 1, // 黑棋标志
    White = 2  // 白棋标志
};

struct Move
{
    int x;     // 行数
    int y;     // 列数
    int score; // 评分
};

/* Define HashEntry type */
struct HashEntry
{
    int depth; // 深度
    int score; // 分数
};

/* Define constant variables */
const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1}; // X 方向偏移量
const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1}; // Y 方向偏移量

const int BOARD_SIZE = 12;    // 棋盘大小

const int INVALID_COORD = -1; // 无效坐标

const int INF = 2147483647;

const int FIVE_LINE = 1000000000; // 五连分数
const int LIVE_FOUR = 1000000;   // 活四分数
const int RUSH_FOUR = 10000;     // 冲四分数
const int LIVE_THREE = 10000;    // 活三分数
const int RUSH_THREE = 100;     // 冲三分数
const int LIVE_TWO = 100;       // 活二分数
const int RUSH_TWO = 10;        // 冲二分数
const int LIVE_ONE = 10;        // 活一分数
const int RUSH_ONE = 1;         // 冲一分数

const int SEARCH_DEPTH = 6; // 搜索深度 -> 3轮

Status myFlag;                        // 我方棋子标志
Status enemyFlag;                     // 对方棋子标志
Status board[BOARD_SIZE][BOARD_SIZE]; // 棋盘信息

int myScore[BOARD_SIZE][BOARD_SIZE];    // 我方分数
int enemyScore[BOARD_SIZE][BOARD_SIZE]; // 对方分数
int stepCount = 0;                      // 步数统计

typedef unsigned long long ULL;                // 64 位无符号整数类型
ULL zobristTable[BOARD_SIZE][BOARD_SIZE][3];   // Zobrist 数
ULL currentZobristHash;                        // 当前 Zobrist 哈希值
std::unordered_map<ULL, HashEntry> transTable; // Zobrist 置换表

// 判断坐标是否在棋盘范围内
inline bool isInBound(const int x, const int y)
{
    return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE);
}

// 更新哈希值
inline void updateZobristHash(const int x, const int y, const Status flag)
{
    currentZobristHash ^= zobristTable[x][y][flag]; // 异或操作
}

// 判断棋子周围一定范围内是否有其他棋子
bool hasNeighbor(const int x, const int y, const int radius = 1, int count = 1)
{
    // 搜索范围
    int startRow = std::max(0, x - radius);
    int endRow = std::min(BOARD_SIZE - 1, x + radius);
    int startCol = std::max(0, y - radius);
    int endCol = std::min(BOARD_SIZE - 1, y + radius);

    for (int i = startRow; i <= endRow; i++)
    {
        for (int j = startCol; j <= endCol; j++)
        {
            if (i == x && j == y)
            {
                continue; // 忽略当前位置
            }
            if (board[i][j] != Empty)
            {
                if (--count <= 0)
                { // 找到足够多的棋子
                    return true;
                }
            }
        }
    }
    return false;
}

// 评估得分
int evaluateState(const int count, const int block, const int empty)
{
    if (empty <= 0)
    {
        if (count >= 5)
        {
            return FIVE_LINE; // 五连
        }
        if (block == 0)
        {
            switch (count)
            {
            case 1:
                return LIVE_ONE; // 活一
            case 2:
                return LIVE_TWO; // 活二
            case 3:
                return LIVE_THREE; // 活三
            case 4:
                return LIVE_FOUR; // 活四
            default:
                break;
            }
        }
        if (block == 1)
        {
            switch (count)
            {
            case 1:
                return RUSH_ONE; // 冲一
            case 2:
                return RUSH_TWO; // 冲二
            case 3:
                return RUSH_THREE; // 冲三
            case 4:
                return RUSH_FOUR; // 冲四
            default:
                break;
            }
        }
    }
    else if (empty == 1 || empty == count - 1)
    {
        if (count >= 6)
        {
            return FIVE_LINE; // 五连
        }
        if (block == 0)
        {
            switch (count)
            {
            case 2:
                return LIVE_TWO; // 活二
            case 3:
                return LIVE_THREE; // 活三
            case 4:
                return RUSH_FOUR; // 冲四
            case 5:
                return LIVE_FOUR; // 活四
            default:
                break;
            }
        }
        if (block == 1)
        {
            switch (count)
            {
            case 2:
                return RUSH_TWO; // 冲二
            case 3:
                return RUSH_THREE; // 冲三
            case 4:
                return RUSH_FOUR; // 冲四
            case 5:
                return RUSH_FOUR; // 冲四
            default:
                break;
            }
        }
    }
    else if (empty == 2 || empty == count - 2)
    {
        if (count >= 7)
        {
            return FIVE_LINE; // 五连
        }
        if (block == 0)
        {
            switch (count)
            {
            case 3:
                return LIVE_THREE; // 活三
            case 4:
                return RUSH_FOUR; // 冲四
            case 5:
                return RUSH_FOUR; // 冲四
            case 6:
                return LIVE_FOUR; // 活四
            default:
                break;
            }
        }
        if (block == 1)
        {
            switch (count)
            {
            case 3:
                return RUSH_THREE; // 冲三
            case 4:
                return RUSH_FOUR; // 冲四
            case 5:
                return RUSH_FOUR; // 冲四
            case 6:
                return LIVE_FOUR; // 活四
            default:
                break;
            }
        }
        if (block == 2)
        {
            switch (count)
            {
            case 4:
                return RUSH_FOUR; // 冲四
            case 5:
                return RUSH_FOUR; // 冲四
            case 6:
                return RUSH_FOUR; // 冲四
            default:
                break;
            }
        }
    }
    else if (empty == 3 || empty == count - 3)
    {
        if (count >= 8)
        {
            return FIVE_LINE; // 五连
        }
        if (block == 0)
        {
            switch (count)
            {
            case 4:
                return LIVE_THREE; // 活三
            case 5:
                return LIVE_THREE; // 活三
            case 6:
                return RUSH_FOUR; // 冲四
            case 7:
                return LIVE_FOUR; // 活四
            default:
                break;
            }
        }
        if (block == 1)
        {
            switch (count)
            {
            case 4:
                return RUSH_FOUR; // 冲四
            case 5:
                return RUSH_FOUR; // 冲四
            case 6:
                return RUSH_FOUR; // 冲四
            case 7:
                return LIVE_FOUR; // 活四
            default:
                break;
            }
        }
        if (block == 2)
        {
            switch (count)
            {
            case 4:
                return RUSH_FOUR; // 冲四
            case 5:
                return RUSH_FOUR; // 冲四
            case 6:
                return RUSH_FOUR; // 冲四
            case 7:
                return RUSH_FOUR; // 冲四
            default:
                break;
            }
        }
    }
    else if (empty == 4 || empty == count - 4)
    {
        if (count >= 9)
        {
            return FIVE_LINE; // 五连
        }
        if (block == 0)
        {
            switch (count)
            {
            case 5:
                return LIVE_FOUR; // 活四
            case 6:
                return LIVE_FOUR; // 活四
            case 7:
                return LIVE_FOUR; // 活四
            case 8:
                return LIVE_FOUR; // 活四
            default:
                break;
            }
        }
        if (block == 1)
        {
            switch (count)
            {
            case 4:
                return RUSH_FOUR; // 冲四
            case 5:
                return RUSH_FOUR; // 冲四
            case 6:
                return RUSH_FOUR; // 冲四
            case 7:
                return RUSH_FOUR; // 冲四
            case 8:
                return LIVE_FOUR; // 活四
            default:
                break;
            }
        }
        if (block == 2)
        {
            switch (count)
            {
            case 5:
                return RUSH_FOUR; // 冲四
            case 6:
                return RUSH_FOUR; // 冲四
            case 7:
                return RUSH_FOUR; // 冲四
            case 8:
                return RUSH_FOUR; // 冲四
            default:
                break;
            }
        }
    }
    else if (empty == 5 || empty == count - 5)
    {
        return FIVE_LINE; // 五连
    }
    return 0;
}

// 通用方向检测函数
int evaluateLine(int x, int y, int dx, int dy, Status flag) {
    int count = 1, block = 0, empty = -1;
    int BOARD_LIMIT = BOARD_SIZE - 1;

    for (int step = 1; ; ++step) {
        int nx = x + dx * step, ny = y + dy * step;
        if (nx < 0 || ny < 0 || nx > BOARD_LIMIT || ny > BOARD_LIMIT) {
            block++;
            break;
        }
        if (board[nx][ny] == Empty) {
            if (empty == -1 && (nx + dx >= 0 && ny + dy >= 0) && board[nx + dx][ny + dy] == flag) {
                empty = count;
                continue;
            } else {
                break;
            }
        } else if (board[nx][ny] == flag) {
            count++;
            continue;
        } else {
            block++;
            break;
        }
    }
    for (int step = 1; ; ++step) {
        int nx = x - dx * step, ny = y - dy * step;
        if (nx < 0 || ny < 0 || nx > BOARD_LIMIT || ny > BOARD_LIMIT) {
            block++;
            break;
        }
        if (board[nx][ny] == Empty) {
            if (empty == -1 && (nx - dx >= 0 && ny - dy >= 0) && board[nx - dx][ny - dy] == flag) {
                empty = 0;
                continue;
            } else {
                break;
            }
        } else if (board[nx][ny] == flag) {
            count++;
            if (empty != -1) empty++;
            continue;
        } else {
            block++;
            break;
        }
    }
    return evaluateState(count, block, empty);
}

// 评估单点得分
int evaluatePoint(int x, int y, Status flag) {
    int score = 0;
    score += evaluateLine(x, y, 0, 1, flag);  // 水平方向
    score += evaluateLine(x, y, 1, 0, flag);  // 垂直方向
    score += evaluateLine(x, y, 1, 1, flag);  // 对角线 左上-右下
    score += evaluateLine(x, y, 1, -1, flag); // 对角线 右上-左下
    return score;
}

// 评估棋盘得分
int evaluateBoard(void)
{
    int result = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == myFlag)
            {
                result += myScore[i][j];
            }
            else if (board[i][j] == enemyFlag)
            {
                result -= enemyScore[i][j];
            }
        }
    }
    return result;
}

// 更新得分
void updateScore(const int x, const int y)
{
    if (board[x][y] == myFlag)
    {
        myScore[x][y] = evaluatePoint(x, y, myFlag);
    }
    else if (board[x][y] == enemyFlag)
    {
        enemyScore[x][y] = evaluatePoint(x, y, enemyFlag);
    }
    else
    {
        myScore[x][y] = 0;
        enemyScore[x][y] = 0;
    }

    // 八方向更新得分
    for (int i = 0; i < 8; i++)
    {
        int nx = x, ny = y;
        while (true)
        {
            nx += dx[i];
            ny += dy[i];
            if (!isInBound(nx, ny))
            {
                break;
            }
            if (board[nx][ny] == myFlag)
            {
                myScore[nx][ny] = evaluatePoint(nx, ny, myFlag);
            }
            else if (board[nx][ny] == enemyFlag)
            {
                enemyScore[nx][ny] = evaluatePoint(nx, ny, enemyFlag);
            }
        }
    }
}

// 落子
void setMove(const int x, const int y, const Status flag = Empty)
{
    updateZobristHash(x, y, board[x][y]); // 更新哈希值
    board[x][y] = flag;
    updateZobristHash(x, y, flag);
    updateScore(x, y);
}

// 寻找随机空位
void findEmptyToPlace(const Status flag, int &bestX, int &bestY)
{
    int emptyPositions[BOARD_SIZE * BOARD_SIZE][2]; // 假设最多有 BOARD_SIZE * BOARD_SIZE 个空位
    int count = 0;

    // 遍历棋盘，收集所有空位
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == Empty)
            {
                emptyPositions[count][0] = i; // 存储行坐标
                emptyPositions[count][1] = j; // 存储列坐标
                count++;
            }
        }
    }

    if (count > 0)
    {
        // 随机选择一个空位
        srand(time(0)); // 初始化随机种子
        int randomIndex = rand() % count; // 在有效空位范围内生成随机索引
        bestX = emptyPositions[randomIndex][0];
        bestY = emptyPositions[randomIndex][1];
    }
}
/*
void findEmptyToPlace(const Status flag, int &bestX, int &bestY)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == Empty)
            {
                bestX = i;
                bestY = j;
                return;
            }
        }
    }
}
*/

// 检查是否有五子连珠
bool checkLine(const int x, const int y, const Status flag)
{
    int i, j, count;
    for (int dir = 4; dir < 8; dir++)
    {
        count = 1;
        i = x + dx[dir];
        j = y + dy[dir];
        while (isInBound(i, j) && board[i][j] == flag)
        {
            count++;
            i += dx[dir];
            j += dy[dir];
        }
        i = x - dx[dir];
        j = y - dy[dir];
        while (isInBound(i, j) && board[i][j] == flag)
        {
            count++;
            i -= dx[dir];
            j -= dy[dir];
        }
        if (count >= 5)
        {
            return true;
        }
    }
    return false;
}

// 检查一条线上是否含给定棋子数
bool checkLine(const int x, const int y, const Status flag, const int checkCount, const int checkEmpty)
{
    int i, j, count, empty;
    for (int dir = 4; dir < 8; dir++)
    {
        count = 1;
        empty = 0;
        i = x + dx[dir];
        j = y + dy[dir];
        while (isInBound(i, j))
        {
            if (board[i][j] == flag)
            {
                count++;
            }
            else if (board[i][j] == Empty)
            {
                empty++;
                break;
            }
            else
            {
                break;
            }
            i += dx[dir];
            j += dy[dir];
        }
        i = x - dx[dir];
        j = y - dy[dir];
        while (isInBound(i, j))
        {
            if (board[i][j] == flag)
            {
                count++;
            }
            else if (board[i][j] == Empty)
            {
                empty++;
                break;
            }
            else
            {
                break;
            }
            i -= dx[dir];
            j -= dy[dir];
        }
        if (count == checkCount && empty == checkEmpty)
        {
            return true;
        }
    }
    return false;
}

// 生成走法
void generateMoves(Move moves[], int &movesLength, const Status flag)
{
    movesLength = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == Empty && hasNeighbor(i, j))
            {
                moves[movesLength].x = i;
                moves[movesLength].y = j;
                moves[movesLength].score = evaluatePoint(i, j, flag);
                movesLength++;
            }
        }
    }

    // 由大到小排序 -> 启发式搜索
    std::sort(moves, moves + movesLength, [](const Move &a, const Move &b)
              { return a.score > b.score; });
}

// 负极大值搜索 + Alpha-Beta 剪枝算法
int alphaBeta(const int depth, int alpha, int beta, Status flag, int &bestX, int &bestY, bool top = false)
{
    /* Check transposition table */
    std::unordered_map<ULL, HashEntry>::iterator it = transTable.find(currentZobristHash);
    if (it != transTable.end() && it->second.depth >= depth)
    {
        return it->second.score;
    }

    /* Evaluate board */
    if (depth == 0)
    {
        int score;
        if (flag == myFlag)
        {
            score = evaluateBoard();
        }
        else
        {
            score = -evaluateBoard();
        }
        transTable[currentZobristHash] = {depth, score};
        return score;
    }

    // 启发式搜索
    int score, bestLocalX = INVALID_COORD, bestLocalY = INVALID_COORD, movesLength = 0;
    Move nextMoves[BOARD_SIZE * BOARD_SIZE];
    generateMoves(nextMoves, movesLength, flag);

    if (flag == myFlag)
    {
        score = -INF;
        for (int k = 0; k < movesLength; k++)
        {
            int i = nextMoves[k].x, j = nextMoves[k].y;
            setMove(i, j, flag); // 落子
            int tmpScore = alphaBeta(depth - 1, alpha, beta, static_cast<Status>(3 - flag), bestX, bestY);
            setMove(i, j); // 撤销落子
            if (tmpScore > score)
            {
                score = tmpScore;
                bestLocalX = i;
                bestLocalY = j;
            }
            alpha = std::max(alpha, score);
            if (beta <= alpha)
            {
                break; // Beta 剪枝
            }
        }
    }
    else
    {
        score = INF;
        for (int k = 0; k < movesLength; k++)
        {
            int i = nextMoves[k].x, j = nextMoves[k].y;
            setMove(i, j, flag); // 落子
            int tmpScore = alphaBeta(depth - 1, alpha, beta, static_cast<Status>(3 - flag), bestX, bestY);
            setMove(i, j); // 撤销落子
            if (tmpScore < score)
            {
                score = tmpScore;
                bestLocalX = i;
                bestLocalY = j;
            }
            beta = std::min(beta, score);
            if (beta <= alpha)
            {
                break; // Beta 剪枝
            }
        }
    }

    if (top && bestLocalX != INVALID_COORD && bestLocalY != INVALID_COORD)
    {
        bestX = bestLocalX;
        bestY = bestLocalY;
    }

    return score;
}

// 尝试下子
bool tryMove(int &nextX, int &nextY, int &score)
{
    /* First move */
    if (stepCount == 4)
    {
        nextX = 4;
        nextY = 4;
        return false;
    }
    if (stepCount == 5)
    {
        if (board[4][7] == Black)
        {
            nextX = 7;
            nextY = 4;
            return false;
        }
        if (board[7][4] == Black)
        {
            nextX = 4;
            nextY = 7;
            return false;
        }
        if (board[4][4] == Black)
        {
            nextX = 5;
            nextY = 7;
            return false;
        }
        if (board[7][7] == Black)
        {
            nextX = 6;
            nextY = 4;
            return false;
        }
    }

    // 寻找我方五连
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == Empty && checkLine(i, j, myFlag))
            {
                nextX = i;
                nextY = j;
                return false;
            }
        }
    }

    // 寻找对手五连
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == Empty && checkLine(i, j, enemyFlag))
            {
                nextX = i;
                nextY = j;
                return false;
            }
        }
    }

    // 寻找我方 活四 或 四三 (LIVE_FOUR || (RUSH_FOUR && LIVE_THREE)) 
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == Empty && (checkLine(i, j, myFlag, 4, 2) || (checkLine(i, j, myFlag, 4, 1) && checkLine(i, j, myFlag, 3, 2))))
            {
                nextX = i;
                nextY = j;
                return false;
            }
        }
    }

    // 寻找对手 活四 或 四三 (LIVE_FOUR || (RUSH_FOUR && LIVE_THREE)) 
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == Empty && (checkLine(i, j, enemyFlag, 4, 2) || (checkLine(i, j, enemyFlag, 4, 1) && checkLine(i, j, enemyFlag, 3, 2))))
            {
                nextX = i;
                nextY = j;
                return false;
            }
        }
    }
    return true;
}

// 生成随机数
ULL randNum()
{
    return rand() ^ ((ULL)rand() << 15) ^ ((ULL)rand() << 30) ^ ((ULL)rand() << 45) ^ ((ULL)rand() << 60);
}

// 初始化Zobrist表
void initZobrist()
{
    srand(time(NULL));
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                zobristTable[i][j][k] = randNum();
            }
        }
    }
}

// 初始化
void init()
{
    memset(board, 0, sizeof(board));
    memset(myScore, 0, sizeof(myScore));
    memset(enemyScore, 0, sizeof(enemyScore));

    initZobrist();
    currentZobristHash = 0;
}

void loop()
{
    char tag[10] = {0};
    while (true)
    {
        memset(tag, 0, sizeof(tag));
        scanf("%s", tag);
        if (!strcmp(tag, "START"))
        {

            scanf("%d", &myFlag);
            enemyFlag = static_cast<Status>(3 - myFlag);
            init();

            // 花月开局
            setMove(5, 5, White);
            setMove(6, 6, White);
            setMove(5, 6, Black);
            setMove(6, 5, Black);
            stepCount += 4;

            /* Respond to terminal program */
            // printf("OK\n");
            // fflush(stdout);
        }
        else if (!strcmp(tag, "PLACE"))
        { 
            Move command = {0, 0, 0};
            scanf("%d %d", &command.x, &command.y);
            setMove(command.x, command.y, enemyFlag);
            stepCount++;
        }
        else if (!strcmp(tag, "TURN"))
        { 

            int nextX = INVALID_COORD, nextY = INVALID_COORD, score = 0;
            if (tryMove(nextX, nextY, score))
            {
                score = alphaBeta(SEARCH_DEPTH, -INF, INF, myFlag, nextX, nextY, true);
            }
            if (nextX == INVALID_COORD || nextY == INVALID_COORD)
            {
                findEmptyToPlace(myFlag, nextX, nextY);
            }
            setMove(nextX, nextY, myFlag);
            printf("%d %d\n", nextX, nextY);
            fflush(stdout);
            stepCount++;
        }
        else if (!strcmp(tag, "END"))
        {
            int status;
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