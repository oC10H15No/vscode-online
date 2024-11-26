/* 2351050 杨瑞晨 */
#include <iostream>
#include <string>
#include <algorithm>     // for reverse()
#include <cmath>
#include <limits>
using namespace std;

// 将六进制字符串转换为十进制整数
int convertBase6ToDec(const string &k) {
    int result = 0;
    for (char c : k) {
        if (c < '0' || c > '5') {  // 确保是合法的六进制字符
            throw invalid_argument("");
        }
        result = 6 * result + (c - '0');
    }
    return result;
}

// 将十进制数n转换为m进制表示
string convertDecToBaseM(int n, int m) {
    if (n == 0) return "0";  // 如果是0，直接返回"0"

    string result = "";
    while (n > 0) {
        int remainder = n % m;  // 取余数
        result += (remainder < 10) ? char(remainder + '0') : char(remainder - 10 + 'A');
        n /= m;  // 进行除法
    }
    reverse(result.begin(), result.end());  // 逆序
    return result;
}

int main() {
    int m, n;
    string k;

    while (true) {
        cout << "请输入目标进制 m (2<=m<=20): ";
        cin >> m;
        // 输入错误处理
        if (cin.fail() || m < 2 || m > 20) {
            cerr << "输入错误！" << endl;
            cin.fail();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            continue;
        } else break;
    }
    while (true) {
        cout << "请输入六进制数字串 k (0<=n<=10^6): ";
        cin >> k;
        bool validInput = true;
        try {
            // 检查 k 是否为合法的六进制数字串
            n = convertBase6ToDec(k);  // 尝试转换
        } catch (const invalid_argument &e) {
            cerr << "输入错误！" << endl;
            cin.fail();
            cin.ignore(INT_MAX, '\n');
            validInput = false;  // 输入不合法
        }
        if (validInput) {
            break;  // 输入合法，退出循环
        }
    }

    // 将六进制数 k 转换为十进制整数
    n = convertBase6ToDec(k);

    // 将 n 转换为 m 进制表示
    string result = convertDecToBaseM(n, m);

    // 输出转换后的结果
    cout << "六进制数字 " << k << " 的 " << m << " 进制表示是: " << result << endl;

    return 0;
}
