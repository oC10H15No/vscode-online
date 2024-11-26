#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>

// board information
#define BOARD_SIZE 12
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define INF 2147483647
#define _INF -2147483647

// other information
const int MAX_DEPTH = 4; // 搜索最大深度
const int MIN_DEPTH = 2;  // 搜索最小深度

// command information
#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"

int myFlag;
int enemyFlag;

const int BOARD_MIDDLE_1 = (BOARD_SIZE + 1) / 2 - 1; // 5
const int BOARD_MIDDLE_2 = BOARD_SIZE / 2;           // 6

struct coordinate
{
    int x = -1;
    int y = -1;
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
};
Nude A;
bool judgeInRange(coordinate temp)
{ // 判断位置是否超出棋盘
    if (temp.x < 0)
        return false;
    if (temp.y < 0)
        return false;
    if (temp.x >= BOARD_SIZE)
        return false;
    if (temp.y >= BOARD_SIZE)
        return false;
    return true;
}
int getColor(coordinate target)
{ // 获取该位置的棋子种类
    return A.board[target.x][target.y];
}
void place(coordinate target)
{ // 放置自己的棋子
    A.board[target.x][target.y] = myFlag;
}
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
            A.board[command.x][command.y] = enemyFlag;
        }
        else if (strcmp(tag, TURN) == 0)
        { // 下在左上角的逻辑
            coordinate leftTop;
            leftTop.x = 0;
            leftTop.y = 0;
            if (judgeInRange(leftTop) && getColor(leftTop) == 0)
            {
                printf("%d %d\n", leftTop.x, leftTop.y);
                fflush(stdout);
                place(leftTop);
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