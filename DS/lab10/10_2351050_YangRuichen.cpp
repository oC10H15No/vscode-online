#include <iostream>
#include <ctime>
#include <limits>
#include <iomanip>

using namespace std;

clock_t startTime, endTime;

class Sort
{
public:
    int size;          // 数据大小
    int count;         // 比较次数
    int *originalData; // 原始数据
    int *currentData;  // 用于排序的数据副本

    int getDigit();                                                             // 获取最大数字的位数
    void merge(bool (*comp)(int, int), int *arr, int left, int mid, int right); // 归并排序的合并操作
    int partition(int *arr, int left, int right, bool (*comp)(int, int));       // 快速排序的分区操作
    void quick_sort(bool (*comp)(int, int), int left, int right);               // 快速排序的递归操作
    void heapify(int *arr, int n, int i, bool (*comp)(int, int));               // 维护堆性质
public:
    Sort(int n) : size(n), count(0)
    {
        originalData = new int[size];
        currentData = new int[size];
        srand(static_cast<unsigned int>(time(nullptr))); // 生成随机数种子
        for (int i = 0; i < size; i++)
            originalData[i] = currentData[i] = rand(); // 生成随机数
    }

    ~Sort()
    {
        delete[] originalData;
        delete[] currentData;
    }

    void swap(int &a, int &b)
    {
        if (a == b)
            return;
        int temp = a;
        a = b;
        b = temp;
    }

    // 重置数据：将 `currentData` 恢复为 `originalData`，并清零计数器
    void reset()
    {
        count = 0;
        for (int i = 0; i < size; i++)
            currentData[i] = originalData[i];
    }

    void print(int *arr)
    {
        for (int i = 0; i < size; i++)
            cout << arr[i] << " ";
        cout << endl;
    }

    void output(const char *prompt, ostream &os = cout)
    {
        os << left;
        os << setw(20) << prompt << "所用时间：" << static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
        os << setw(20) << prompt << "交换次数：" << count << endl;
        os << endl;
    }

    void bubble_sort(bool (*comp)(int, int));                     // 冒泡排序
    void select_sort(bool (*comp)(int, int));                     // 选择排序
    void insert_sort(bool (*comp)(int, int));                     // 插入排序
    void shell_sort(bool (*comp)(int, int));                      // 希尔排序
    void merge_sort(bool (*comp)(int, int), int left, int right); // 归并排序
    void quick_sort(bool (*comp)(int, int));                      // 快速排序
    void heap_sort(bool (*comp)(int, int));                       // 堆排序
    void radix_sort();                                            // 基数排序
};

// 冒泡排序
void Sort::bubble_sort(bool (*comp)(int, int))
{
    reset();
    for (int i = 0; i < size - 1; ++i)
    {
        for (int j = 0; j < size - i - 1; ++j)
        {
            if (comp(currentData[j], currentData[j + 1]))
            {
                swap(currentData[j], currentData[j + 1]);
                count++;
            }
        }
    }
}

// 选择排序
void Sort::select_sort(bool (*comp)(int, int))
{
    reset();
    for (int i = 0; i < size - 1; ++i)
    {
        int min_index = i;
        for (int j = i + 1; j < size; ++j)
        {
            if (comp(currentData[min_index], currentData[j]))
                min_index = j;
            count++;
        }
        if (min_index != i)
        {
            swap(currentData[i], currentData[min_index]);
        }
    }
}

// 插入排序
void Sort::insert_sort(bool (*comp)(int, int))
{
    reset();
    for (int i = 1; i < size; ++i)
    {
        int temp = currentData[i];
        int j = i - 1;
        while (j >= 0 && comp(currentData[j], temp))
        {
            currentData[j + 1] = currentData[j];
            count++;
            j--;
        }
        currentData[j + 1] = temp;
    }
}

// 希尔排序
void Sort::shell_sort(bool (*comp)(int, int))
{
    reset();
    for (int gap = size / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < size; ++i)
        {
            int temp = currentData[i];
            int j;
            for (j = i; j >= gap && comp(currentData[j - gap], temp); j -= gap)
            {
                currentData[j] = currentData[j - gap];
                count++;
            }
            currentData[j] = temp;
        }
    }
}

// 归并排序的合并操作
void Sort::merge(bool (*comp)(int, int), int *arr, int left, int mid, int right)
{
    int subArr1 = mid - left + 1;
    int subArr2 = right - mid;
    // 创建临时数组
    int *leftArr = new int[subArr1];
    int *rightArr = new int[subArr2];
    for (int i = 0; i < subArr1; i++)
        leftArr[i] = arr[left + i];
    for (int i = 0; i < subArr2; i++)
        rightArr[i] = arr[mid + 1 + i];

    // 合并两个子数组
    int i = 0, j = 0, k = left;
    while (i < subArr1 && j < subArr2)
    {
        if (!comp(leftArr[i], rightArr[j]))
        {
            arr[k++] = leftArr[i++];
        }
        else
        {
            arr[k++] = rightArr[j++];
        }
        count++;
    }
    // 将剩余元素复制到 arr
    while (i < subArr1)
    {
        arr[k++] = leftArr[i++];
    }
    while (j < subArr2)
    {
        arr[k++] = rightArr[j++];
    }

    delete[] leftArr;
    delete[] rightArr;
}

// 归并排序
void Sort::merge_sort(bool (*comp)(int, int), int left, int right)
{
    static bool firstCall = true;
    if (firstCall)
    {
        reset();
        firstCall = false;
    }

    if (left < right)
    {
        int mid = left + (right - left) / 2;
        merge_sort(comp, left, mid);
        merge_sort(comp, mid + 1, right);
        merge(comp, currentData, left, mid, right);
    }
}

// 快速排序
void Sort::quick_sort(bool (*comp)(int, int))
{
    reset();
    quick_sort(comp, 0, size - 1);
}

// 快速排序的分区操作
int Sort::partition(int *arr, int left, int right, bool (*comp)(int, int))
{
    int pivot = arr[right];
    int i = left - 1;
    for (int j = left; j < right; j++)
    {
        if (comp(pivot, arr[j]))
        {
            i++;
            swap(arr[i], arr[j]);
            count++;
        }
    }
    swap(arr[i + 1], arr[right]);
    count++;
    return i + 1;
}

void Sort::quick_sort(bool (*comp)(int, int), int left, int right)
{
    if (left < right)
    {
        int pi = partition(currentData, left, right, comp);
        quick_sort(comp, left, pi - 1);
        quick_sort(comp, pi + 1, right);
    }
}

// 维护堆性质
void Sort::heapify(int *arr, int n, int i, bool (*comp)(int, int))
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && comp(arr[left], arr[largest]))
        largest = left;
    if (right < n && comp(arr[right], arr[largest]))
        largest = right;
    if (largest != i)
    {
        swap(arr[i], arr[largest]);
        count++;
        heapify(arr, n, largest, comp);
    }
}

// 堆排序
void Sort::heap_sort(bool (*comp)(int, int))
{
    reset();
    for (int i = size / 2 - 1; i >= 0; --i)
        heapify(currentData, size, i, comp);
    for (int i = size - 1; i > 0; --i)
    {
        swap(currentData[0], currentData[i]);
        count++;
        heapify(currentData, i, 0, comp);
    }
}

// 获取最大数字的位数
int Sort::getDigit()
{
    int res = 1;
    int maxNum = originalData[0];
    for (int i = 1; i < size; i++)
    {
        if (originalData[i] > maxNum)
            maxNum = originalData[i];
    }

    while (maxNum /= 10)
        res++;

    return res;
}

// 基数排序
void Sort::radix_sort()
{
    reset();

    int digit = getDigit();
    int radix = 1;

    int *countNum = new int[10]; // 计数数组
    int *output = new int[size]; // 输出数组

    for (int i = 0; i < digit; i++)
    {
        // 初始化计数数组
        for (int j = 0; j < 10; j++)
            countNum[j] = 0;
        // 统计当前位的数字频率
        for (int j = 0; j < size; j++)
        {
            countNum[(currentData[j] / radix) % 10]++;
            count++;
        }
        // 累计计数、确定结束位置
        for (int j = 1; j < 10; j++)
            countNum[j] += countNum[j - 1];
        // 根据当前位数字，将数据放到输出数组中
        for (int j = size - 1; j >= 0; j--)
            output[--countNum[(currentData[j] / radix) % 10]] = currentData[j];
        // 排序结果拷贝到原数组
        for (int j = 0; j < size; j++)
            currentData[j] = output[j];

        radix *= 10;
    }

    delete[] countNum;
    delete[] output;
}

int main()
{
    cout << "**            排序算法比较              **" << endl;
    cout << "==========================================" << endl;
    cout << "**        1---冒泡排序                  **" << endl;
    cout << "**        2---选择排序                  **" << endl;
    cout << "**        3---直接插入排序              **" << endl;
    cout << "**        4---希尔排序                  **" << endl;
    cout << "**        5---快速排序                  **" << endl;
    cout << "**        6---堆排序                    **" << endl;
    cout << "**        7---归并排序                  **" << endl;
    cout << "**        8---基数排序(只支持从小到大)  **" << endl;
    cout << "**        9---退出程序                  **" << endl;
    cout << "==========================================" << endl;
    cout << endl;

    int size;
    while (1)
    {
        cout << "请输入要产生的随机数的个数: ";
        cin >> size;
        if (cin.fail() || size <= 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "非法输入，请重新输入！" << endl;
            continue;
        }
        else
            break;
    }

    Sort sort(size);

    bool (*comp)(int left, int right) = [](int left, int right) -> bool
    { return left > right; };
    char order;
    cout << "请选择从小到大排序还是从大到小排序(输入 L/l 代表从小到大排序，G/g 代表从大到小排序): ";
    while (true)
    {
        cin >> order;
        if (cin.fail() || (order != 'L' && order != 'l' && order != 'G' && order != 'g'))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "非法输入，请重新输入！" << endl;
            continue;
        }
        else if (order == 'G' || order == 'g')
        {
            comp = [](int left, int right) -> bool
            { return left < right; };
            break;
        }
        else
            break;
    }

    int choice = -1;
    while (true)
    {
        while (true)
        {
            cout << "请选择排序算法: ";
            cin >> choice;
            if (cin.fail() || choice < 1 || choice > 9)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "非法输入，请重新输入！" << endl;
                continue;
            }
            else
                break;
        }
        // sort.print(sort.originalData);
        switch (choice)
        {
        case 1:
            startTime = clock();
            sort.bubble_sort(comp);
            endTime = clock();
            sort.output("冒泡排序");
            break;
        case 2:
            startTime = clock();
            sort.select_sort(comp);
            endTime = clock();
            sort.output("选择排序");
            break;
        case 3:
            startTime = clock();
            sort.insert_sort(comp);
            endTime = clock();
            sort.output("直接插入排序");
            break;
        case 4:
            startTime = clock();
            sort.shell_sort(comp);
            endTime = clock();
            sort.output("希尔排序");
            break;
        case 5:
            startTime = clock();
            sort.quick_sort(comp);
            endTime = clock();
            sort.output("快速排序");
            break;
        case 6:
            startTime = clock();
            sort.heap_sort(comp);
            endTime = clock();
            sort.output("堆排序");
            break;
        case 7:
            startTime = clock();
            sort.merge_sort(comp, 0, size - 1);
            endTime = clock();
            sort.output("归并排序");
            break;
        case 8:
            startTime = clock();
            sort.radix_sort();
            endTime = clock();
            sort.output("基数排序");
            break;
        case 9:
            return 0;
        default:
            break;
        }
        // sort.print(sort.currentData);
    }

    return 0;
}