#include <iostream>
#include <limits>
#include <cstring>

using namespace std;

const int STRING_SIZE = 32;

// 边
class Edge
{
public:
    int start, end;   // 顶点
    double weight; // 权重
    bool operator<(const Edge &other) const { return weight < other.weight; }
    bool operator>(const Edge &other) const { return weight > other.weight; }
    Edge() {}
    Edge(int u, int v, double weight) : start(u), end(v), weight(weight) {}
};

// 点
struct Vertex
{
public:
    char name[STRING_SIZE]; // 名称
    int ancestor;  // 记录在并查集中的祖先
    Vertex() {}
    Vertex(char *name, int ancestor) : ancestor(ancestor) { strcpy(this->name, name); }
};

// 快速排序模版函数
template <class T>
void quickSort(T *arr, int low, int high)
{
    if (low > high) return;
    T mid = arr[(low + high) / 2];
    int i = low, j = high;
    while (i < j) {
        while (arr[i] < mid) ++i;
        while (arr[j] > mid) --j;
        if (i <= j) {
            swap(arr[i], arr[j]);
            ++i;
            --j;
        }
    }
    if (low < j) quickSort(arr, low, j);
    if (i < high) quickSort(arr, i, high);
}

// 图
class Graph
{
    template <class T>
    friend void quickSort(T *arr, int low, int high);
private:
    int V;           // 顶点数
    int E;           // 边数
    Vertex *vertexes;  // 顶点数组
    Edge *edges;     // 边数组
    Graph *mst;      // 最小生成树

    // 根据名称查找顶点
    int findVertex(char *name)
    {
        for (int i = 0; i < V; ++i)
        {
            if (strcmp(name, vertexes[i].name) == 0)
            {
                return i;
            }
        }
        return -1;
    }
    // 在并查集中查找顶点祖先
    int findAncestor(int i)
    {
        if (vertexes[i].ancestor != i)
        {
            vertexes[i].ancestor = findAncestor(vertexes[i].ancestor); // 路径压缩
        }
        return vertexes[i].ancestor;
    }

    // 重置图
    void reset() {
        V = 0, E = 0;
        if (vertexes) delete[] vertexes;
        if (edges) delete[] edges;
        if (mst) delete mst;
        vertexes = nullptr;
        edges = nullptr;
    }

    // 根据名称快速排序边
    void quickSortEdgesByName(Edge edges[], int left, int right, Vertex vertexes[])
    {
        if (left >= right) return;
    
        int i = left, j = right;
        Edge pivot = edges[left];
    
        while (i < j)
        {
            // 从右向左，找到起点名称小于基准的边
            while (i < j && strcmp(vertexes[edges[j].start].name, vertexes[pivot.start].name) >= 0)
                j--;
            if (i < j)
                edges[i++] = edges[j];
    
            // 从左向右，找到起点名称大于基准的边
            while (i < j && strcmp(vertexes[edges[i].start].name, vertexes[pivot.start].name) <= 0)
                i++;
            if (i < j)
                edges[j--] = edges[i];
        }
    
        edges[i] = pivot;
    
        quickSortEdgesByName(edges, left, i - 1, vertexes);
        quickSortEdgesByName(edges, i + 1, right, vertexes);
    }
public:
    Graph(int V = 0, int E = 0) : V(V), E(E)
    {   
        edges = nullptr;
        vertexes = nullptr;
        mst = nullptr;
        edges = new Edge[E];
        vertexes = new Vertex[V];
    }

    ~Graph()
    {
        delete[] vertexes;
        delete[] edges;
        delete mst;
    }

    void printMST()
    {
        if (mst == nullptr)
        {
            cerr << "请先构造最小生成树！" << endl;
            return;
        }

        quickSortEdgesByName(mst->edges, 0, mst->E - 1, vertexes);
        
        cout << "最小生成树的顶点及边如下：" << endl;
        
        for (int i = 0; i < mst->E; ++i)
        {
            cout << vertexes[mst->edges[i].start].name << " - (" << mst->edges[i].weight << ") -> " << vertexes[mst->edges[i].end].name << endl;
        }
    }

    bool kruskalMST()
    {
        // 按权重从小到大排序
        quickSort(edges, 0, E - 1);

        delete mst;
        mst = nullptr;
        mst = new Graph(V, V - 1);

        int selected_edges = 0;
        for (int i = 0; i < E; ++i)
        {
            int u = findAncestor(edges[i].start); // 查找边的两个顶点的祖先
            int v = findAncestor(edges[i].end);
            if (u != v) 
            {
                mst->edges[selected_edges++] = edges[i]; // 加入最小生成树
                vertexes[u].ancestor = v; // 合并并查集
            }
            if (selected_edges == V - 1) {
                return true; // 最小生成树构造完成
            }
        }

        delete mst;
        mst = nullptr;
        return false;
    }

    void initVertexes()
    {
        reset();

        cout << "请输入顶点的个数：";
        while (true)
        {
            cin >> V;
            if (cin.fail() || V < 2)
            {
                cerr << "输入错误，请重新输入！" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            else
                break;
        }
        vertexes = new Vertex[V];

        cout << "请依次输入各顶点的名称：" << endl;
        for (int i = 0; i < V; ++i)
        {   
            char name[STRING_SIZE];
            cout << "第 " << i + 1 << " 个顶点：";
            cin >> name;
            if (findVertex(name) != -1)
            {
                cerr << "顶点名称重复，请重新输入！" << endl;
                --i;
            }
            else
            {
                vertexes[i] = Vertex(name, i);
            }
        }
    }

    void initEdges()
    {
        delete[] edges;

        cout << "请输入边的个数：";
        while (true)
        {
            cin >> E;
            if (cin.fail() || E < V - 1 || E > V * (V - 1) / 2)
            {
                cerr << "输入错误，请重新输入" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            else
                break;
        }
        edges = new Edge[E];

        cout << "请依次输入两个顶点及边：" << endl;
        for (int i = 0; i < E; ++i)
        {
            char u[STRING_SIZE], v[STRING_SIZE];
            double weight;
            while (true)
            {
                cin >> u >> v;
                if (findVertex(u) == -1 || findVertex(v) == -1)
                {
                    cerr << "未找到顶点，请重新输入！" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                else
                    break;
            }

            while (true)
            {
                cin >> weight;
                if (cin.fail() || weight < 0)
                {
                    cerr << "权重输入错误，请重新输入！" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                else
                    break;
            }

            edges[i] = Edge(findVertex(u), findVertex(v), weight);
        }
    }
};

void printMenu()
{
    /*.      0102030405060708091011121314151617181920*/
    cout << "*********** 电网造价模拟系统 ***********" << endl;
    cout << "========================================" << endl;
    cout << "**        A --- 创建电网顶点          **" << endl;
    cout << "**        B --- 添加电网的边          **" << endl;
    cout << "**        C --- 构造最小生成树        **" << endl;
    cout << "**        D --- 显示最小生成树        **" << endl;
    cout << "**        E --- 退出程序              **" << endl;
    cout << "========================================" << endl;
}

int main()
{
    printMenu();
    Graph g;
    while (true)
    {
        char ch;
        cout << "请选择操作：";
        while (true)
        {
            cin >> ch;
            if (ch >= 'A' && ch <= 'E')
                break;
            else
            {
                cerr << "输入错误，请重新输入！" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        switch (ch)
        {
        case 'A':
            g.initVertexes();
            break;
        case 'B':
            g.initEdges();
            break;
        case 'C':
            if (!g.kruskalMST())
            {
                cerr << "构造最小生成树失败！" << endl;
            }
            else
            {
                cout << "构造最小生成树成功！" << endl;
            }
            break;
        case 'D':
            g.printMST();
            break;
        case 'E':
            return 0;
            break;
        }
    }

    return 0;
}

/*
a b 8
b c 7
c d 5
d a 11
a c 18
b d 12
*/
