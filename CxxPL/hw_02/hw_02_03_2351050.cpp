/* 2351050 杨瑞晨 */
#include <iostream>
#include <list>
#include <limits>
using namespace std;

const int kLuckyNumber = 7;

// 判断数字是否含7
bool containSeven(int n)
{
    while (n)
    {
        if (n % 10 == kLuckyNumber)
            return true;
        n /= 10;
    }
    return false;
}

// 吃巧克力
void eatChocolate(list<int> &chocolate)
{
    // 初始化迭代器指向链表的第一个元素
    list<int>::iterator it = chocolate.begin();
    int count = 1;
    while (chocolate.size() > 1)
    {
        if (count % kLuckyNumber == 0 || containSeven(count))
        {
            // 保存当前迭代器的位置，以便移除后更新
            auto toRemove = it;
            // 移动迭代器到下一个位置
            ++it;
            // 如果迭代器到达末尾，则循环回到开头
            if (it == chocolate.end())
                it = chocolate.begin();
            // 从链表中移除
            chocolate.erase(toRemove);
        }
        else
        {
            // 不移除，移动迭代器
            ++it;
            // 如果迭代器到达末尾，则循环回到开头
            if (it == chocolate.end())
                it = chocolate.begin();
        }
        ++count;
    }
}

int main()
{
    int num;
    while (true)
    {
        cout << "Please input the number of the chocolate: ";
        cin >> num;
        // 输入错误处理
        if (cin.fail() || num <= 0)
        {
            cerr << "The input must be an integer over 0!" << endl;
            cin.fail();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        else
            break;
    }

    // 初始化链表，包含1到n的编号
    list<int> chocolate;
    for (int i = 1; i <= num; ++i)
    {
        chocolate.push_back(i);
    }

    eatChocolate(chocolate);

    cout << chocolate.front() << endl;

    return 0;
}