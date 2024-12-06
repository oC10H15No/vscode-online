#include <iostream>
#include <limits>
using namespace std;

const int MAX_SIZE = 12;

class Maze;
class MazeCell;
class MazePath;

bool isBorder(int x, int y, int size)
{
    return x == 0 || x == size - 1 || y == 0 || y == size - 1;
}

bool isCorner(int x, int y, int size)
{
    return (x == 0 && y == 0) || (x == 0 && y == size - 1) || (x == size - 1 && y == 0) || (x == size - 1 && y == size - 1);
}

// 迷宫单元类
class MazeCell
{
    friend class Maze;
    friend class MazePath;

private:
    int x;        // x 坐标
    int y;        // y 坐标
    bool visited; // 是否访问过
    bool wall;    // 墙
public:
    MazeCell(int x = 0, int y = 0, bool visited = false, bool wall = false) : x(x), y(y), visited(visited), wall(wall) {} // 构造函数
    MazeCell(const MazeCell &cell) : x(cell.x), y(cell.y), visited(cell.visited), wall(cell.wall) {}                      // 拷贝构造函数
    ~MazeCell() {}                                                                                                        // 析构函数
    int getX() { return x; }                                                                                              // 获取 x 坐标
    int getY() { return y; }                                                                                              // 获取 y 坐标
    bool isVisited() { return visited; }                                                                                  // 是否访问过
    bool isWall() { return wall; }                                                                                        // 是否是墙
    void setVisited(bool visited) { this->visited = visited; }                                                            // 设置是否访问过
    void setWall(bool wall) { this->wall = wall; }                                                                        // 设置是否是墙
    MazeCell &operator=(const MazeCell &cell)
    { // 赋值运算符重载
        if (this != &cell)
        {
            x = cell.x;
            y = cell.y;
            visited = cell.visited;
            wall = cell.wall;
        }
        return *this;
    }
};

// 迷宫路径栈，用于存放路径
class MazePath
{
    friend class Maze;
    friend class MazeCell;

private:
    int top;                              // 栈顶
    MazeCell *stack[MAX_SIZE * MAX_SIZE]; // 栈
public:
    MazePath() { top = -1; } // 构造函数
    ~MazePath() {}           // 析构函数
    void push(MazeCell *cell)
    { // 入栈
        if (top < MAX_SIZE * MAX_SIZE - 1)
        {
            stack[++top] = cell;
        }
    }
    MazeCell *pop()
    { // 出栈
        if (top >= 0)
        {
            return stack[top--];
        }
        return nullptr;
    }
    MazeCell *getTop()
    { // 获取栈顶元素
        if (top >= 0)
        {
            return stack[top];
        }
        return nullptr;
    }
    bool isEmpty()
    { // 判断栈是否为空
        return top == -1;
    }
    void clear()
    { // 清空栈
        top = -1;
    }
    void printPath()
    { // 打印路径
        for (int i = 0; i <= top; i++)
        {
            cout << "(" << stack[i]->getX() << ", " << stack[i]->getY() << ")" << (i == top ? "" : " ---> ");
        }
        cout << endl;
    }
};

// 迷宫类
class Maze
{
    friend class MazePath;
    friend class MazeCell;

private:
    int size;         // 迷宫大小
    MazeCell **cells; // 迷宫单元
    MazeCell start;   // 起点
    MazeCell end;     // 终点
    MazePath path;    // 路径
public:
    Maze(int size = 0) : size(size), start(MazeCell()), end(MazeCell()), path(MazePath())
    { // 构造函数
        cells = new MazeCell *[size];
        for (int i = 0; i < size; i++)
        {
            cells[i] = new MazeCell[size];
        }
    }
    ~Maze()
    { // 析构函数
        for (int i = 0; i < size; i++)
        {
            delete[] cells[i];
        }
        delete[] cells;
    }
    void inputMaze()
    { // 输入迷宫
        cout << "请输入迷宫(0: 通道, 1: 墙): " << endl;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                int n;
                cin >> n;
                cells[i][j] = MazeCell(i, j);
                if (n == 1) // 墙
                {
                    cells[i][j].setWall(true);
                }
            }
        }
        cout << "请输入起点: x y (范围: [0, " << size - 1 << "]): ";
        int x, y;
        cin >> x >> y;
        if (!isBorder(x, y, size) || cells[x][y].isWall() || isCorner(x, y, size))
        {
            cerr << "非法起点！起点必须在边界上且不能在四个角落，并且不能是墙。" << endl;
            exit(1);
        }
        start = cells[x][y];
        // cout << start.getX() << " " << start.getY() << endl;

        // 检查出口的唯一性
        int exitCount = 0;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (isBorder(i, j, size) && !cells[i][j].isWall() && !(i == start.getX() && j == start.getY()) && !isCorner(i, j, size))
                {
                    exitCount++;
                    end = cells[i][j];
                }
            }
        }
        if (exitCount != 1)
        {
            cerr << "迷宫必须有且仅有一个出口，并且出口不能在四个角落！" << endl;
            exit(1);
        }
    }
    void printMaze()
    { // 打印迷宫
        cout << endl
             << "迷宫如下: 起点为(" << start.getX() << ", " << start.getY() << "), 终点为(" << end.getX() << ", " << end.getY() << ")" << endl;
        cout << "    ";
        for (int i = 0; i < size; i++)
        {
            cout << "C" << i << (i > 9 ? " " : "  ");
        }
        cout << endl;
        for (int i = 0; i < size; i++)
        {
            cout << "R" << i << (i > 9 ? " " : "  ");
            for (int j = 0; j < size; j++)
            {
                if (cells[i][j].isWall())
                {
                    cout << "#";
                }
                else if (cells[i][j].isVisited())
                {
                    cout << "*";
                }
                else
                {
                    cout << "0";
                }
                cout << "   ";
            }
            cout << endl
                 << endl;
        }
    }
    void solution()
    {
        if (dfsFindPath(start.getX(), start.getY())) // 查找路径
        {
            printMaze();
            cout << "路径如下: " << endl;
            path.printPath();
        }
        else
        {
            cout << "没有找到路径!" << endl;
            return;
        }
    }
    bool dfsFindPath(int x, int y)
    { // DFS 算法查找路径

        if (x < 0 || x >= size || y < 0 || y >= size || cells[x][y].isWall() || cells[x][y].isVisited()) // 越界或者是墙或者已经访问过
        {
            // cout << "当前位置: (" << x << ", " << y << ") 无法继续前进" << endl;
            return false; // 越界或者是墙或者已经访问过
        }

        // cout << "当前位置: (" << x << ", " << y << ")" << endl;

        cells[x][y].setVisited(true); // 标记为已访问
        path.push(&cells[x][y]);      // 入栈

        if (isBorder(x, y, size) && (x != start.getX() || y != start.getY())) // 到达终点
        {
            end = MazeCell(x, y);
            return true;
        }

        if (dfsFindPath(x - 1, y) || // 向上
            dfsFindPath(x + 1, y) || // 向下
            dfsFindPath(x, y - 1) || // 向左
            dfsFindPath(x, y + 1))   // 向右
        {
            return true;
        }

        // cout << "当前位置: (" << x << ", " << y << ") 无法继续前进, 回溯" << endl;

        cells[x][y].setVisited(false); // 标记为未访问
        path.pop();                    // 出栈
        return false;
    }
};

int getInputSize()
{
    int size;
    cout << "请输入迷宫大小(范围[3, " << MAX_SIZE << "]): ";
    while (true)
    {
        cin >> size;
        if (cin.fail() || size < 3 || size > MAX_SIZE)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cerr << "输入错误, 请重新输入: ";
        }
        else
        {
            break;
        }
    }
    return size;
}

int main()
{
    Maze maze(getInputSize());
    maze.inputMaze();
    maze.solution();

    return 0;
}

/*
测试数据

eins:(legal)
7
1 0 1 1 1 1 1
1 0 1 0 0 0 1
1 0 1 0 1 1 1
1 0 0 0 1 0 1
1 0 1 0 0 0 1
1 0 1 0 1 0 1
1 1 1 1 1 0 1
0 1

zwei:(legal)
7
1 1 0 1 1 1 1
1 0 0 0 0 0 1
1 0 1 0 1 1 1
1 0 0 0 1 0 0
1 0 1 0 0 0 1
1 0 1 0 1 0 1
1 1 1 1 1 1 1
3 6

drei:(illegal) 没有出口
7
1 1 0 1 1 1 1
1 0 0 0 0 0 1
1 0 1 0 1 1 1
1 0 0 0 1 0 1
1 0 1 0 0 0 1
1 0 1 0 1 0 1
1 1 1 1 1 1 1
0 2

vier:(illegal) 两个出口
7
1 1 0 1 1 1 1
1 0 0 0 0 0 1
1 0 1 0 1 1 1
1 0 0 0 1 0 1
1 0 1 0 0 0 1
1 0 1 0 1 0 1
1 0 1 1 1 0 1
0 2

funf:(illegal) 起点在角落
5
0 1 1 1 1
0 0 0 1 1
1 1 0 1 1
1 0 0 0 0
1 1 1 1 1
0 0

sechs:(legal)
12
1 0 1 1 1 1 1 1 1 1 1 1
1 0 0 0 0 0 0 0 0 0 0 1
1 0 1 1 1 1 1 1 1 1 0 1
1 0 1 0 0 0 0 0 0 1 0 1
1 0 1 0 1 1 1 1 0 1 0 1
1 0 1 0 1 0 0 1 0 1 0 1
1 0 1 0 1 0 1 1 0 1 0 1
1 0 1 0 1 0 1 0 0 1 0 1
1 0 1 0 1 0 1 0 1 1 0 1
1 0 1 0 1 0 1 0 0 0 0 1
1 0 0 0 0 0 0 0 1 1 1 1
1 1 1 1 1 1 0 1 1 1 1 1
0 1
*/