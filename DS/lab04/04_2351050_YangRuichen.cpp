#include <iostream>
#include <limits>
#include <cstring>
#include <cstdio>

using namespace std;

const int DEFAULT_CAPACITY = 10;
const int STRING_SIZE = 50;

template <typename T>
class Stack
{
private:
    T *data;      // 栈元素数组
    int topIndex; // 栈顶索引
    int capacity; // 栈容量

    void resize(int newCapacity)
    {
        // 重新分配更大的数组
        T *newData = new T[newCapacity];
        for (int i = 0; i < topIndex; ++i)
        {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Stack(int initialCapacity = DEFAULT_CAPACITY) : topIndex(0), capacity(initialCapacity) { data = new T[capacity]; } // 构造函数

    ~Stack() { delete[] data; } // 析构函数

    void push(const T &value) // 入栈
    {
        // 如果栈满，扩容
        if (topIndex == capacity)
        {
            resize(2 * capacity);
        }
        data[topIndex++] = value; // 入栈
    }

    void pop() // 出栈
    {
        // 如果栈空，抛出异常
        if (isEmpty())
        {
            throw std::out_of_range("Stack underflow");
        }
        --topIndex; // 出栈
    }

    T &top() // 获取栈顶元素
    {
        // 如果栈空，抛出异常
        if (isEmpty())
        {
            throw std::out_of_range("Stack is empty");
        }
        return data[topIndex - 1]; // 返回栈顶元素
    }

    bool isEmpty() const // 判断栈是否为空
    {
        return topIndex == 0;
    }

    int size() const // 获取栈大小
    {
        return topIndex;
    }

    friend ostream &operator<<(ostream &out, const Stack<T> &stack)
    {
        for (int i = 0; i < stack.topIndex; ++i)
        {
            out << stack.data[i] << " ";
        }
        return out;
    }
};

// 判断是否为数字
bool isdigit(char ch)
{
    return ch >= '0' && ch <= '9';
}

// 数字转字符串
char *numToString(double num)
{
    char *str = new char[STRING_SIZE];
    sprintf(str, "%g", num); // 使用 sprintf 将数字转换为字符串
    return str;
}

// 字符转字符串
char *charToString(char ch)
{
    char *str = new char[2];
    str[0] = ch;
    str[1] = '\0';
    return str;
}

// 表达式树
class ExpressionTree
{
private:
    struct TreeNode
    {
        char *data;           // 数据
        TreeNode *left;       // 左孩子
        TreeNode *right;      // 右孩子
        bool hasLeftBracket;  // 是否有左括号
        bool hasRightBracket; // 是否有右括号
        TreeNode(char *data, TreeNode *left = nullptr, TreeNode *right = nullptr) : data(data), left(left), right(right), hasLeftBracket(false), hasRightBracket(false) {}
    };

    TreeNode *root; // 根节点

    // 运算符优先级
    int precedence(char op)
    {
        if (op == '+' || op == '-')
            return 1;
        else if (op == '*' || op == '/' || op == '%')
            return 2;
        else
            return 0;
    }

    void clear(TreeNode *node) // 清空树
    {
        if (node == nullptr)
        {
            return;
        }
        clear(node->left);
        clear(node->right);
        delete node;
    }

    TreeNode buildTree(const char *input)
    {
        Stack<TreeNode *> nodes;
        Stack<char> optrs;
        bool nextHasLeftBracket = false;

        int i = 0;
        int n = strlen(input);
        while (i < n)
        {
            if (input[i] == ' ')
            { // 跳过空格
                ++i;
                continue;
            }

            if (isdigit(input[i]) || ((input[i] == '-' || input[i] == '+') && isdigit(input[i + 1])))
            {
                double num = 0;

                int negtiveFlag = 1;     // 负数标志 是负数置 -1
                bool doubleFlag = false; // 小数标志
                double decimal = 1;      // 小数位数

                if (input[i] == '-')
                {
                    negtiveFlag = -1;
                    ++i;
                }
                else if (input[i] == '+')
                {
                    ++i;
                }

                while (i < n && (isdigit(input[i]) || input[i] == '.'))
                {
                    if (input[i] == '.')
                    {
                        doubleFlag = true;
                        ++i;
                        continue;
                    }
                    if (!doubleFlag)
                    {
                        num = num * 10 + input[i] - '0'; // 计算整数部分
                    }
                    else
                    {
                        decimal *= 0.1;
                        num += (input[i] - '0') * decimal; // 计算小数部分
                    }
                    ++i;
                }
                // nums.push(num * negtiveFlag); // 入栈
                char *numStr = numToString(num * negtiveFlag);
                TreeNode *node = new TreeNode(numStr);
                node->hasLeftBracket = nextHasLeftBracket;
                nextHasLeftBracket = false;
                nodes.push(node); // 将数字推入栈
                ++i;
            }
            else if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '%' || input[i] == '(' || input[i] == ')')
            {
                if (input[i] == '(')
                {
                    nextHasLeftBracket = true;
                    optrs.push(input[i]); // '(' 直接入栈
                }
                else if (input[i] == ')')
                {
                    while (!optrs.isEmpty() && optrs.top() != '(') // 将 '(' 之前的运算符弹出
                    {
                        createSubTree(nodes, optrs);
                    }
                    optrs.pop(); // 弹出 '('
                    if (!nodes.isEmpty())
                    {
                        nodes.top()->hasRightBracket = true;
                    }
                }
                else
                {
                    while (!optrs.isEmpty() && precedence(optrs.top()) >= precedence(input[i])) // 弹出优先级高的运算符
                    {
                        createSubTree(nodes, optrs);
                    }
                    optrs.push(input[i]); // 入栈
                }
                ++i;
            }
        }
        // 将剩余的运算符弹出

        while (!optrs.isEmpty())
        {
            createSubTree(nodes, optrs);
        }
        return *nodes.top();
    }

    void createSubTree(Stack<TreeNode *> &nodes, Stack<char> &optrs)
    {
        char optr = optrs.top(); // 获取栈顶运算符
        optrs.pop(); // 弹出栈顶运算符
        TreeNode *right = nodes.top(); // 获取右子树
        nodes.pop(); // 弹出右子树
        TreeNode *left = nodes.top(); // 获取左子树
        nodes.pop(); // 弹出左子树
        TreeNode *curr = new TreeNode(charToString(optr)); // 创建当前运算符节点
        curr->left = left; // 设置左子树
        curr->right = right; // 设置右子树
        nodes.push(curr); // 将当前节点推入栈
    }

    void preorder(TreeNode *node) // 前序遍历
    {
        if (node == nullptr)
            return;
        // if (node->hasLeftBracket) cout << "(";
        cout << node->data << " ";
        preorder(node->left);
        preorder(node->right);
        // if (node->hasRightBracket) cout << ")";
    }

    void inorder(TreeNode *node) // 中序遍历
    {
        if (node == nullptr)
            return;
        if (node->hasLeftBracket)
            cout << "( ";
        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
        if (node->hasRightBracket)
            cout << ") ";
    }

    void postorder(TreeNode *node) // 后序遍历
    {
        if (node == nullptr)
            return;
        // if (node->hasLeftBracket) cout << "(";
        postorder(node->left);
        postorder(node->right);
        cout << node->data << " ";
        // if (node->hasRightBracket) cout << ")";
    }

public:
    ExpressionTree(const char *input) : root(nullptr)
    {
        root = new TreeNode(nullptr);
        *root = buildTree(input);
    }

    void printPreorder() // 前序遍历
    {
        cout << "波兰表达式：";
        preorder(root);
        cout << endl;
    }

    void printPostorder() // 后序遍历
    {
        cout << "逆波兰表达式：";
        postorder(root);
        cout << endl;
    }
};

int main()
{
    cout << "请输入中缀表达式(以空格分隔不同对象，不超过 20 个字符): " << endl;
    char input[STRING_SIZE];
    cin.getline(input, STRING_SIZE, '\n');

    ExpressionTree et(input);
    et.printPreorder();
    et.printPostorder();

    return 0;
}

/*
测试用例
2 + 3 * ( 7 - 4 ) + 8 / 4
1314 + 25.5 * 12
-2 * ( +3 )
123
( ( 2 + 3 ) * 4 - ( 8 + 2 ) ) / 5
*/