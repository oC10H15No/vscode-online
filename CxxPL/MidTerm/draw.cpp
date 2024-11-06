#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

// 棋盘大小
const int BOARD_SIZE = 12;

// 棋盘数组
int board[BOARD_SIZE][BOARD_SIZE];

// 大脑程序的进程ID
pid_t brain_pid = -1;

// 大脑程序的输入输出文件描述符
int brain_in_fd[2];
int brain_out_fd[2];

// 函数声明
void initBoard();
void drawBoard();
void updateBoard(int x, int y, int player);
void startBrain();
void stopBrain();
void sendCommandToBrain(const char* command);
void getMoveFromBrain(int &x, int &y);


int myFlag;
int enemyFlag;

int main()
{
    // 初始化棋盘
    initBoard();

    // 启动大脑程序
    startBrain();

    char input[100];
    bool isRunning = true;
    while (isRunning)
    {
        // 显示提示信息
        cout << "请输入命令 : ";
        // 读取用户输入
        cin.getline(input, sizeof(input));

        // 根据命令处理
        if (strncmp(input, "START", 5) == 0)
        {
            // 发送命令给大脑程序
            sendCommandToBrain(input);

            

            sscanf(input + 6, "%d", &myFlag);
            enemyFlag = 3 - myFlag;
            
            updateBoard(5, 5, 2); // AI 落子
            updateBoard(5, 6, 1); // 玩家落子
            updateBoard(6, 6, 2); // AI 落子
            updateBoard(6, 5, 1); // 玩家落子
            drawBoard();
           
        }
        else if (strncmp(input, "PLACE", 5) == 0)
        {
            int x, y;
            sscanf(input + 6, "%d %d", &x, &y);
            if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[y][x] == 0)
            {
                updateBoard(x, y, enemyFlag); // 玩家落子
                drawBoard();

                // 将玩家的落子发送给大脑程序
                sendCommandToBrain(input);
            }
            else
            {
                cout << "无效的位置！" << endl;
            }
        }
        else if (strncmp(input, "TURN", 4) == 0)
        {
            // 发送命令给大脑程序
            sendCommandToBrain(input);

            // 获取大脑程序的下棋位置
            int aiX, aiY;
            getMoveFromBrain(aiX, aiY);
            if (aiX >= 0 && aiX < BOARD_SIZE && aiY >= 0 && aiY < BOARD_SIZE && board[aiY][aiX] == 0)
            {
                updateBoard(aiY, aiX, myFlag); // AI 落子（注意坐标顺序）
                drawBoard();
            }
            else
            {
                cout << "AI 返回了无效的位置！" << endl;
            }
        }
        else if (strncmp(input, "END", 3) == 0)
        {
            // 发送命令给大脑程序
            sendCommandToBrain(input);

            isRunning = false;
        }
        else
        {
            cout << "未知的命令！" << endl;
        }
    }

    // 停止大脑程序
    stopBrain();

    return 0;
}

void initBoard()
{
    memset(board, 0, sizeof(board));
}

void drawBoard()
{
    system("clear");
    cout << "当前棋盘状态：" << endl;
    cout << "  ";
    for (int x = 0; x < BOARD_SIZE; x++)
    {
        cout << x << (x < 10 ? " " : "");
    }
    cout << endl;
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        cout << y << (y < 10 ? " " : "");
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (board[y][x] == 0)
            {
                cout << "  ";
            }
            else if (board[y][x] == 1)
            {
                cout << "○ ";
            }
            else if (board[y][x] == 2)
            {
                cout << "● ";
            }
        }
        cout << endl;
    }
}

void updateBoard(int x, int y, int player)
{
    board[y][x] = player;
}

void startBrain()
{
    // 创建管道
    if (pipe(brain_in_fd) == -1 || pipe(brain_out_fd) == -1)
    {
        perror("pipe");
        exit(1);
    }

    brain_pid = fork();
    if (brain_pid < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (brain_pid == 0)
    {
        // 子进程 - 执行大脑程序
        dup2(brain_in_fd[0], STDIN_FILENO);   // 将 stdin 重定向到 brain_in_fd 读取端
        dup2(brain_out_fd[1], STDOUT_FILENO); // 将 stdout 重定向到 brain_out_fd 写入端

        // 关闭不需要的文件描述符
        close(brain_in_fd[0]);
        close(brain_in_fd[1]);
        close(brain_out_fd[0]);
        close(brain_out_fd[1]);

        // 执行大脑程序（确保 6.0-1 可执行）
        //execl("./gomoku", "./gomoku", (char *)NULL);
        execl("./gomoku_t", "./gomoku_t", (char *)NULL);
        //execl("./gomoku_c", "./gomoku_c", (char *)NULL);
        //execl("./6.0-1", "./6.0-1", (char *)NULL);

        // 如果 execl 失败
        perror("execl");
        exit(1);
    }
    else
    {
        // 父进程
        // 关闭不需要的文件描述符
        close(brain_in_fd[0]);  // 父进程不需要读取大脑程序的 stdin
        close(brain_out_fd[1]); // 父进程不需要写入大脑程序的 stdout
    }
}

void stopBrain()
{
    if (brain_pid > 0)
    {
        kill(brain_pid, SIGTERM);
        waitpid(brain_pid, NULL, 0);
    }
}

void sendCommandToBrain(const char* command)
{
    write(brain_in_fd[1], command, strlen(command));
    // 发送命令后添加换行符
    write(brain_in_fd[1], "\n", 1);
    // 确保写入完成
    fsync(brain_in_fd[1]);
}

void getMoveFromBrain(int &x, int &y)
{
    char buffer[128];
    int index = 0;
    char ch;
    // 读取大脑程序的输出，直到遇到换行符
    while (read(brain_out_fd[0], &ch, 1) == 1)
    {
        if (ch == '\n')
        {
            break;
        }
        buffer[index++] = ch;
    }
    buffer[index] = '\0';
    // 解析大脑程序的输出，注意坐标顺序
    sscanf(buffer, "%d %d", &y, &x); // 大脑程序返回的格式为 "row col"
}