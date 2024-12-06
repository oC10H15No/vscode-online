#include <iostream>
#include <limits>
using namespace std;

template <class T>
class List;

// 定义链表节点类
template <class T>
class ListNode {
    friend class List<T>;
    template <class>
    friend class ListIterator;
private:
    T data;         // 节点数据
    ListNode *next; // 指向下一个节点的指针
public:
    ListNode() : next(nullptr) {}  // 默认构造函数
    ListNode(T val) : data(val), next(nullptr) {} // 带参数的构造函数
};

// 定义链表迭代器类
template <typename T>
class ListIterator {
public:
    ListNode<T> *current; // 当前节点指针

    ListIterator(ListNode<T> *node) : current(node) {} // 构造函数

    // 重载 != 操作符，用于比较两个迭代器是否不相等
    bool operator!=(const ListIterator &other) const {
        return current != other.current;
    }

    // 重载 * 操作符，用于获取当前节点的数据
    T &operator*() {
        return current->data;
    }

    // 重载 ++ 操作符，用于将迭代器移动到下一个节点
    ListIterator &operator++() {
        current = current->next;
        return *this;
    }
};

// 定义链表类
template <class T>
class List {
private:
    ListNode<T> *head; // 链表头指针
    ListNode<T> *tail; // 链表尾指针
public:
    List() : head(nullptr), tail(nullptr) {}                     // 默认构造函数
    List(T val) { head = new ListNode<T>(val), tail = nullptr; } // 带参数的构造函数
    ~List() { clear(); }                                         // 析构函数，清空链表

    void push_back(T val) {
        ListNode<T> *newNode = new ListNode<T>(val);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    // 判断链表是否为空
    bool empty() const {
        return head == nullptr;
    }

    // 清空链表
    void clear() {
        ListNode<T> *current = head;
        while (current) {
            ListNode<T> *next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
    }

    // 返回指向链表头部的迭代器
    ListIterator<T> begin() const {
        return ListIterator<T>(head);
    }

    // 返回指向链表尾部的迭代器
    ListIterator<T> end() const {
        return ListIterator<T>(nullptr);
    }

    // 计算链表长度
    int size() const {
        int count = 0;
        ListNode<T> *current = head;
        while (current) {
            count++;
            current = current->next;
        }
        return count;
    }

    // 搜索含数据 val 的元素
    ListNode<T> *search(T val) {
        ListNode<T> *current = head;
        while (current) {
            if (current->data == val) return current; // 找到
            current = current->next;
        }
        return nullptr; // 没找到返回空指针
    }

    // 搜索第 i 个元素的地址
    ListNode<T> *locate(int i) {
        if (i < 0) return nullptr;
        ListNode<T> *current = head;
        int count = 0;
        while (current && count < i) {
            current = current->next;
            count++;
        }
        return current; // 返回第 i 个元素的地址
    }

    // 取出第 i 个元素的值
    T *getData(int i) {
        if (i < 0) return nullptr;
        ListNode<T> *node = locate(i);
        if (node) return &(node->data); // 返回寻找到的结点地址
        return nullptr; // 搜索不成功返回空值
    }

    // 用 val 修改第 i 个元素的值
    void setData(int i, T &val) {
        if (i < 0) return;
        ListNode<T> *node = locate(i);
        if (node) node->data = val;
    }

    // 将 val 插入到链表第 i 个元素
    bool insert(int i, T &val) {
        if (i < 0) return false; // 非法值
        ListNode<T> *newNode = new ListNode<T>(val); // 创建节点
        if (i == 0) { // 插入到头节点
            newNode->next = head; // 重新链接
            head = newNode;
            if (!tail) tail = newNode;
            return true; // 成功插入
        }
        ListNode<T> *prev = locate(i - 1); // 定位第 i-1 个元素
        if (prev) {
            newNode->next = prev->next; // 重新链接
            prev->next = newNode;
            if (!newNode->next) {
                tail = newNode; // 插入值后无元素
            }
            return true; // 成功插入
        }
        delete newNode; // 删除
        return false; // 未成功插入返回 false
    }

    // 从链表中删去第 i 个元素
    bool remove(int i) {
        if (i < 0 || !head) return false; // 非法值
        if (i == 0) {
            ListNode<T> *temp = head;
            head = head->next;
            delete temp;
            if (!head) tail = nullptr;
            return true; // 成功删除
        }
        ListNode<T> *prev = locate(i - 1); // 定位第 i-1 个元素
        if (prev && prev->next) { // 前后非空
            ListNode<T> *temp = prev->next; // 将删除元素链接到 temp
            prev->next = temp->next;
            if (!prev->next) {
                tail = prev; // 更新尾指针
            }
            delete temp; // 删除 temp
            return true; // 成功删除
        }
        return false; // 未成功删除
    }

    // 重载赋值操作符
    List<T> &operator=(List<T> &L) {
        if (this != &L) {
            clear();
            ListNode<T> *current = L.head;
            while (current) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;
    }
};

// 读取用户输入，生成链表
List<int> readList() {
    List<int> lst;
    int num;

    bool validInput = false;

    while (!validInput) {
        validInput = true;

        while (true) {
            cin >> num;
            // 检查输入是否合法
            if (cin.fail() || (num < -1 && num != 0)) {
                cerr << "请输入正整数并以-1结束！" << endl;
                validInput = false;
                lst.clear();
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else if (num == -1) {
                break;
            } else {
                lst.push_back(num);
            }
        }
    }

    return lst;
}

// 寻找两个链表的交集
List<int> findIntersectionLists(const List<int> &lst1, const List<int> &lst2) {
    List<int> result;

    ListIterator<int> it1 = lst1.begin();
    ListIterator<int> it2 = lst2.begin();

    // 遍历两个链表，寻找交集
    while (it1 != lst1.end() && it2 != lst2.end()) {
        if (*it1 == *it2) {
            result.push_back(*it1);
            ++it1;
            ++it2;
        } else if (*it1 > *it2) {
            ++it2;
        } else {
            ++it1;
        }
    }

    return result;
}

// 打印链表
void printList(const List<int> &lst) {
    if (lst.empty()) {
        cout << "NULL" << endl;
    } else {
        for (ListIterator<int> it = lst.begin(); it != lst.end(); ++it) {
            if (it != lst.begin()) {
                cout << " ";
            }
            cout << *it;
        }
        cout << endl;
    }
}

int main() {
    cout << "请输入第一个链表（若干个正整数构成的非降序序列，用-1表示结尾，数字用空格间隔）：";
    List<int> lst1 = readList(); // 读取第一个链表
    cout << "请输入第二个链表：";
    List<int> lst2 = readList(); // 读取第二个链表
    List<int> result = findIntersectionLists(lst1, lst2); // 寻找交集
    cout << "二链表交集为：";
    printList(result); // 打印交集

    return 0;
}

/*
1 2 5 -1
2 4 5 8 10 -1

1 3 5 -1
2 4 6 8 10 -1

1 2 3 4 5 -1
1 2 3 4 5 -1

3 5 7 -1
2 3 4 5 6 7 8 -1

-1
10 100 1000 -1

*/



