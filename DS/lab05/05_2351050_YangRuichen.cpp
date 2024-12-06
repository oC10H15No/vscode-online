#include <iostream>

using namespace std;

const int DEFAULT_CAPACITY = 10;

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
    void enQueue(const T &value) // 入队
    {
        // 如果队满，扩容
        if (size() == capacity - 1)
        {
            resize(2 * capacity);
        }
        data[rearIndex] = value; // 入队
        rearIndex = (rearIndex + 1) % capacity;
    }
    T deQueue() // 出队
    {
        // 如果队空，抛出异常
        if (isEmpty())
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
        if (isEmpty())
        {
            throw std::out_of_range("Queue is empty");
        }
        return data[frontIndex];
    }
    bool isEmpty() const // 判断队是否为空
    {
        return frontIndex == rearIndex;
    }
    bool isFull() const // 判断队是否为满
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

void getInput(Queue<int> &A, Queue<int> &B)
{
    cout << "请输入队列，其中第一个数为顾客总数 n, 接下来 n 个数为顾客编号(数字间以空格分隔) : " << endl;
    int n;
    cin >> n;
    if (cin.fail() || n <= 0)
    {
        throw std::invalid_argument("Invalid input");
        exit(1);
    }

    for (int i = 0; i < n; ++i)
    {
        int num;
        cin >> num;
        if (cin.fail() || num <= 0)
        {
            throw std::invalid_argument("Invalid input");
            exit(1);
        }

        if (num % 2 == 0)
        {
            B.enQueue(num);
        }
        else
        {
            A.enQueue(num);
        }
    }
}

void handleQueue(Queue<int> &A, Queue<int> &B)
{
    cout << "业务处理顺序为: ";
    while (!A.isEmpty() || !B.isEmpty())
    {
        // A 处理的速度是 B 的两倍，且优先输出 A
        if (!A.isEmpty())
        {
            cout << A.deQueue() << (A.size() == 0 && B.size() == 0 ? "" : " "); // 保证队尾不输出空格
        }
        if (!A.isEmpty())
        {
            cout << A.deQueue() << (A.size() == 0 && B.size() == 0 ? "" : " ");
        }
        if (!B.isEmpty())
        {
            cout << B.deQueue() << (A.size() == 0 && B.size() == 0 ? "" : " ");
        }
    }
    cout << endl;
}

int main()
{
    Queue<int> A, B;

    getInput(A, B);
    handleQueue(A, B);

    return 0;
}

/*
8 2 1 3 9 4 11 13 15
8 2 1 3 9 4 11 12 16
20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
1 6
*/