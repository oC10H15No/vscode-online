/* 2351050 杨瑞晨 */
#include <iostream>
#include <limits>
using namespace std;

// 计算符合条件的数字串个数
int countValidNumbers(int n) {
    // 偶数位置可以是 {0, 2, 4, 6, 8}, 共 5 种选择
    // 奇数位置可以是 {2, 3, 5, 7}, 共 4 种选择
    const int kEvenChoices = 5; // 偶数位选择数
    const int kPrimeChoices = 4; // 奇数位选择数

    int totalCount = 1; // 计数总数，初始化为1以便后续乘法

    // 遍历
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            // 偶数位置
            totalCount *= kEvenChoices;
        } 
        else {
            // 奇数位置
            totalCount *= kPrimeChoices;
        }
    }

    return totalCount;
}

int main() {
    int n;
    while (true) {
        cout << "Please input the length of the password n: ";
        cin >> n;
        // 输入错误处理
        if (cin.fail() || n <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            cerr << "The input must be an integer over 0!" << endl;
            continue;
        }
        else break;
    }

    cout << "The password is " << countValidNumbers(n) <<  "." << endl;

    return 0;
}