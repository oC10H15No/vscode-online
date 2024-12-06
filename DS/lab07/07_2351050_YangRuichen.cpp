#include <iostream>
#include <limits>

using namespace std;

const int maxPQSize = 50; // 优先队列的最大容量

// 定义一个模板类 PriorityQueue，用于实现优先队列
template <typename T, typename Compare = less<T>>
class PriorityQueue
{
private:
    T *heap;      // 存储堆元素的数组
    int capacity; // 堆的容量
    int count;    // 当前堆中元素的数量
    Compare comp; // 比较器对象，用于定义堆的优先级规则

    // 上滤操作，维护堆的性质
    void heapifyUp(int index)
    {
        while (index > 0) // 如果当前节点不是根节点
        {
            int parent = (index - 1) / 2; // 计算父节点的索引
            if (!comp(heap[parent], heap[index]))
                break;                            // 如果当前节点不优于父节点，停止上滤
            std::swap(heap[index], heap[parent]); // 交换当前节点和父节点的位置
            index = parent;                       // 更新当前节点索引为父节点索引
        }
    }

    // 下滤操作，维护堆的性质
    void heapifyDown(int index)
    {
        while (index < count) // 确保当前节点有子节点
        {
            int left = 2 * index + 1;  // 左子节点索引
            int right = 2 * index + 2; // 右子节点索引
            int largest = index;       // 假设当前节点是最大的

            if (left < count && comp(heap[largest], heap[left]))
            {
                largest = left; // 如果左子节点更优，更新 largest
            }
            if (right < count && comp(heap[largest], heap[right]))
            {
                largest = right; // 如果右子节点更优，更新 largest
            }
            if (largest == index)
                break; // 如果当前节点已经是最优的，停止下滤

            std::swap(heap[index], heap[largest]); // 交换当前节点和更优子节点
            index = largest;                       // 更新当前节点索引为更优子节点的索引
        }
    }

    // 扩容操作，增加堆的容量
    void resize()
    {
        capacity *= 2;                // 容量加倍
        T *newHeap = new T[capacity]; // 创建一个更大容量的数组
        for (int i = 0; i < count; ++i)
        {
            newHeap[i] = heap[i]; // 复制旧堆元素到新堆中
        }
        delete[] heap;  // 释放旧堆内存
        heap = newHeap; // 更新堆指针为新堆
    }

public:
    // 构造函数，初始化堆的容量和元素数量
    PriorityQueue(int cap = maxPQSize, Compare comparator = Compare())
        : capacity(cap), count(0), comp(comparator)
    {
        heap = new T[capacity]; // 分配容量为 cap 的数组
    }

    // 析构函数，释放堆数组的内存
    ~PriorityQueue()
    {
        delete[] heap;
    }

    // 插入元素到堆中
    void push(const T &value)
    {
        if (count == capacity) // 如果堆满了
        {
            resize(); // 扩容
        }
        heap[count] = value; // 将新元素放到堆的末尾
        heapifyUp(count);    // 上滤操作，维护堆的性质
        ++count;             // 增加堆中元素数量
    }

    // 删除堆顶元素
    void pop()
    {
        if (count == 0) // 如果堆为空
        {
            throw std::out_of_range("PriorityQueue is empty"); // 抛出异常
        }
        heap[0] = heap[--count]; // 将最后一个元素移到堆顶
        heapifyDown(0);          // 下滤操作，维护堆的性质
    }

    // 获取堆顶元素
    T top() const
    {
        if (count == 0) // 如果堆为空
        {
            throw std::out_of_range("PriorityQueue is empty"); // 抛出异常
        }
        return heap[0]; // 返回堆顶元素
    }

    // 判断堆是否为空
    bool empty() const
    {
        return count == 0;
    }

    // 获取堆中元素的数量
    size_t size() const
    {
        return count;
    }
};

int total_cost = 0; // 全局变量，用于存储总花费

// 霍夫曼树节点结构
struct HuffmanNode
{
    int freq;           // 频率，表示权值
    HuffmanNode *left;  // 左子树指针
    HuffmanNode *right; // 右子树指针

    HuffmanNode(int freq) : freq(freq), left(nullptr), right(nullptr) {}
};

// 自定义比较器，用于霍夫曼节点的比较
struct CompareHuffmanNode
{
    bool operator()(HuffmanNode *a, HuffmanNode *b)
    {
        return a->freq > b->freq; // 小频率优先
    }
};

// 初始化霍夫曼树节点队列
PriorityQueue<HuffmanNode *, CompareHuffmanNode> initializeTree(int freq[], int size)
{
    PriorityQueue<HuffmanNode *, CompareHuffmanNode> minHeap(maxPQSize); // 最小堆
    for (int i = 0; i < size; ++i)
    {
        minHeap.push(new HuffmanNode(freq[i])); // 将频率数组的元素转换为霍夫曼节点并插入堆中
    }
    return minHeap;
}

// 构建霍夫曼树
HuffmanNode *buildHuffmanTree(PriorityQueue<HuffmanNode *, CompareHuffmanNode> &minHeap)
{
    HuffmanNode *left, *right, *top;

    while (minHeap.size() != 1) // 当堆中剩余元素不止一个
    {
        left = minHeap.top(); // 取出堆顶的最小频率节点作为左子节点
        minHeap.pop();

        right = minHeap.top(); // 取出堆顶的次小频率节点作为右子节点
        minHeap.pop();

        top = new HuffmanNode(left->freq + right->freq); // 创建新节点，其频率为左右子节点频率之和
        top->left = left;                                // 左子树连接
        top->right = right;                              // 右子树连接

        total_cost += top->freq; // 累加新节点的频率到总花费

        minHeap.push(top); // 将新节点插入堆中
    }

    return minHeap.top(); // 返回最终的霍夫曼树根节点
}

int main()
{
    int n;
    cout << "请输入木头要锯成的块数(正整数): ";
    while (true)
    {
        cin >> n;
        if (cin.fail() || n <= 0)
        {
            cin.clear();                                         // 清除输入流状态
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 跳过错误输入
            cout << "输入错误，请重新输入: ";
            continue;
        }
        else
        {
            break;
        }
    }

    int *freq = new int[n]; // 动态分配数组存储频率

    cout << "请依次输入每块木头的长度: ";
    for (int i = 0; i < n; ++i)
    {
        cin >> freq[i];
        if (cin.fail() || freq[i] <= 0)
        {
            cerr << "输入错误，请重新输入第 " << i + 1 << " 块及以后的木头的长度: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            --i; // 回退索引以重新输入
        }
    }

    auto minHeap = initializeTree(freq, n);        // 初始化最小堆
    HuffmanNode *root = buildHuffmanTree(minHeap); // 构建霍夫曼树
    cout << "总花费: " << total_cost << endl;      // 输出总花费

    delete[] freq; // 释放动态分配的内存

    return 0;
}
