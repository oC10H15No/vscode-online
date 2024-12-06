#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

const int MAX_N = 100;  // 最大任务交接点数量
const int MAX_M = 1000; // 最大子任务数量
const int INF = 1e9;    // 无穷大，用于初始化

// 边结构体，表示一个子任务
struct Edge
{
    int from, to, weight; // 子任务的开始交接点、结束交接点和所需时间
    int index;            // 子任务的编号
};

const int DEFAULT_CAPACITY = 10;

// 自定义队列类，用于拓扑排序中的队列操作

template <typename T>
class Queue
{
private:
    T *data;        // 队列元素数组
    int frontIndex; // 队头索引
    int rearIndex;  // 队尾索引
    int capacity;   // 队列容量

    void resize(int newCapacity)
    {
        // 重新分配更大的数组
        T *newData = new T[newCapacity];
        for (int i = 0; i < size(); ++i)
        {
            newData[i] = data[(frontIndex + i) % capacity];
        }
        delete[] data;
        data = newData;
        frontIndex = 0;
        rearIndex = size();
        capacity = newCapacity;
    }

public:
    // 构造函数
    Queue(int initialCapacity = DEFAULT_CAPACITY) : frontIndex(0), rearIndex(0), capacity(initialCapacity)
    {
        data = new T[capacity];
    }
    // 析构函数
    ~Queue()
    {
        delete[] data;
    }
    void push(const T &value) // 入队
    {
        // 如果队满，扩容
        if (size() == capacity - 1)
        {
            resize(2 * capacity);
        }
        data[rearIndex] = value; // 入队
        rearIndex = (rearIndex + 1) % capacity;
    }
    T pop() // 出队
    {
        // 如果队空，抛出异常
        if (empty())
        {
            throw std::out_of_range("Queue underflow");
        }
        T value = data[frontIndex]; // 出队
        frontIndex = (frontIndex + 1) % capacity;
        return value;
    }
    T &front() // 获取队头元素
    {
        // 如果队空，抛出异常
        if (empty())
        {
            throw std::out_of_range("Queue is empty");
        }
        return data[frontIndex];
    }
    bool empty() const // 判断队是否为空
    {
        return frontIndex == rearIndex;
    }
    bool full() const // 判断队是否为满
    {
        return (rearIndex + 1) % capacity == frontIndex;
    }
    int size() const // 获取队大小
    {
        return (rearIndex - frontIndex + capacity) % capacity;
    }
    void clear() // 清空队
    {
        frontIndex = rearIndex = 0;
    }
};

// 图类，用于表示任务调度图
class Graph
{
private:
    int N, M;      // 任务交接点数量和子任务数量
    Edge *tasks;   // 子任务数组，保存每个子任务的开始交接点、结束交接点和所需时间
    int **adj;     // 邻接矩阵，表示任务之间的依赖关系和时间
    int *inDegree; // 入度数组，表示每个任务的前置任务数量

    int *earliest; // 最早开始时间数组
    int *latest;   // 最晚开始时间数组

    int *topoOrder; // 拓扑排序结果数组

public:
    // 构造函数，初始化图
    Graph(int n, int m) : N(n), M(m)
    {
        tasks = new Edge[M];    // 创建子任务数组
        adj = new int *[N + 1]; // 创建邻接矩阵
        for (int i = 1; i <= N; ++i)
        {
            adj[i] = new int[N + 1];
            for (int j = 1; j <= N; ++j)
            {
                adj[i][j] = INF; // 初始化邻接矩阵，表示没有连接
            }
        }
        inDegree = new int[N + 1];         // 创建入度数组
        earliest = new int[N + 1];         // 创建最早开始时间数组
        latest = new int[N + 1];           // 创建最晚开始时间数组
        fill(latest, latest + N + 1, INF); // 初始化最晚开始时间数组为无穷大
        topoOrder = new int[N];            // 创建拓扑排序结果数组
    }

    // 析构函数，释放动态内存
    ~Graph()
    {
        delete[] tasks;
        for (int i = 1; i <= N; ++i)
        {
            delete[] adj[i];
        }
        delete[] adj;
        delete[] inDegree;
        delete[] earliest;
        delete[] latest;
        delete[] topoOrder;
    }

    // 添加子任务信息，并更新邻接矩阵和入度数组
    void addTask(int index, int start, int end, int time)
    {
        tasks[index] = {start, end, time, index}; // 将子任务信息存储到任务数组中
        adj[start][end] = time;                   // 更新邻接矩阵，表示任务 start 到任务 end 所需时间为 time
        inDegree[end]++;                          // 更新入度数组，任务 end 的入度加1，表示有一个任务依赖于它
    }

    // 拓扑排序函数，返回拓扑排序是否成功
    bool topologicalSort()
    {
        Queue<int> q;
        // 将所有入度为 0 的节点入队
        for (int i = 1; i <= N; ++i)
        {
            if (inDegree[i] == 0)
            {
                q.push(i);
            }
        }

        int index = 0; // 用于记录拓扑排序结果的下标
        // 遍历队列，执行拓扑排序
        while (!q.empty())
        {
            int u = q.front();      // 获取队列头部元素
            q.pop();                // 弹出队列头部元素
            topoOrder[index++] = u; // 将节点 u 加入拓扑排序结果

            // 遍历所有与节点 u 相邻的节点，更新它们的入度
            for (int v = 1; v <= N; ++v)
            {
                if (adj[u][v] != INF)
                { // 如果存在边 u -> v
                    if (--inDegree[v] == 0)
                    {
                        q.push(v); // 将入度变为 0 的节点入队
                    }
                }
            }
        }

        // 如果拓扑排序结果包含所有节点，则返回 true；否则返回 false（图中有环，无法拓扑排序）
        return index == N;
    }

    // 计算最早开始时间和最晚开始时间
    void calculateTimes()
    {
        // 计算最早开始时间（正向遍历拓扑排序结果）
        for (int i = 0; i < N; ++i)
        {
            int u = topoOrder[i];
            // 遍历所有与 u 相邻的节点，更新最早开始时间
            for (int v = 1; v <= N; ++v)
            {
                if (adj[u][v] != INF)
                { // 如果存在边 u -> v
                    earliest[v] = max(earliest[v], earliest[u] + adj[u][v]);
                }
            }
        }

        // 计算项目总时间（即最早开始时间的最大值）
        int projectTime = *max_element(earliest + 1, earliest + N + 1);
        fill(latest, latest + N + 1, projectTime); // 将所有节点的最晚开始时间初始化为项目总时间

        // 计算最晚开始时间（反向遍历拓扑排序结果）
        for (int i = N - 1; i >= 0; --i)
        {
            int u = topoOrder[i];
            // 遍历所有与 u 相邻的节点，更新最晚开始时间
            for (int v = 1; v <= N; ++v)
            {
                if (adj[u][v] != INF)
                { // 如果存在边 u -> v
                    latest[u] = min(latest[u], latest[v] - adj[u][v]);
                }
            }
        }
    }

    // 找出所有关键活动并输出
    void findCriticalActivities()
    {
        Edge criticalActivities[MAX_M]; // 存储关键活动
        int criticalCount = 0;          // 关键活动计数

        // 遍历所有子任务，判断是否是关键活动
        for (int i = 0; i < M; ++i)
        {
            int u = tasks[i].from, v = tasks[i].to, w = tasks[i].weight;
            // 判断是否是关键活动：最早开始时间 + 任务时间 = 最晚开始时间
            if (w == latest[v] - earliest[u])
            {
                criticalActivities[criticalCount++] = tasks[i];
            }
        }

        // 输出项目总时间（即最早开始时间的最大值）
        int projectTime = *max_element(earliest + 1, earliest + N + 1);
        cout << "项目总时间: " << projectTime << endl;

        // 对关键活动按照开始节点编号升序，起始节点相同的按照输入顺序降序排序
        sort(criticalActivities, criticalActivities + criticalCount, [](const Edge &a, const Edge &b)
             {
            if (a.from != b.from) return a.from < b.from;
            return a.index > b.index; });

        cout << "关键活动: " << endl;
        // 输出所有关键活动
        for (int i = 0; i < criticalCount; ++i)
        {
            cout << criticalActivities[i].from << " -> " << criticalActivities[i].to << endl;
        }
    }
};

// 主函数
int main()
{
    int N, M;
    cout << "请输入任务交接点数量和子任务数量: ";
    while (true)
    {
        cin >> N >> M; // 输入任务交接点数量 N 和子任务数量 M
        if (cin.fail() || N <= 0 || M <= 0)
        {
            cerr << "输入无效，请重新输入: " << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            break;
        }
    }

    Graph graph(N, M); // 创建图对象

    // 输入所有子任务的起点、终点和所需时间，并添加到图中
    cout << "请依次输入任务的开始和完成的交接点编号以及所需时间: " << endl;
    for (int i = 0; i < M; ++i)
    {
        int start, end, time;
        
        cin >> start >> end >> time;
        if (cin.fail() || start <= 0 || start > N || end <= 0 || end > N || time <= 0)
        {
            cerr << "输入无效，请重新输入第 " << i + 1 << " 个子任务: " << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            --i;
            continue;
        }
        else graph.addTask(i, start, end, time);
    }

    // 进行拓扑排序，如果图中有环，则输出 0 并结束程序
    if (!graph.topologicalSort())
    {
        cout << 0 << endl;
        return 0;
    }

    // 计算最早开始时间和最晚开始时间
    graph.calculateTimes();
    // 找出并输出所有关键活动
    graph.findCriticalActivities();

    return 0;
}

/*
7 8
1 2 4
1 3 3
2 4 5
3 4 3
4 5 2
4 6 6
5 7 5
6 7 2

9 11
1 2 6
1 3 4
1 4 5
2 5 1
3 5 1
4 6 2
5 7 9
5 8 7
6 8 4
7 9 2
8 9 4

4 5
1 2 4
2 3 5
3 4 6
4 2 3
4 1 2
*/