/* 2351050 杨瑞晨 */
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

string add(string& str1, string& str2) {
    // 使 num1 和 num2 的长度相同，短的前面补0
    if (str1.length() < str2.length()) {
        swap(str1, str2);
    }
    str2.insert(str2.begin(), str1.length() - str2.length(), '0');

    string result = "";  // 存储结果
    int carry = 0;       // 进位

    // 从最低位开始逐位相加
    for (int i = str1.length() - 1; i >= 0; --i) {
        int digit1 = str1[i] - '0';
        int digit2 = str2[i] - '0';
        int sum = digit1 + digit2 + carry;
        carry = sum / 10;  // 计算新的进位
        result.push_back((sum % 10) + '0');  // 将当前位的和存入结果中
    }
    // 如果最后还有进位，加入结果
    if (carry) {
        result.push_back(carry + '0');
    }

    // 反转结果
    reverse(result.begin(), result.end());

    return result;
}

int main() {
    int m, n;
    string str1, str2;
    cout << "请依次键入整数 A 的位数、整数 A 、整数 B 的位数、整数 B ：" << endl;
    cin >> m >> str1 >> n >> str2;

    string result = add(str1, str2);
    cout << "A+B 的结果是 " << result << endl;

    return 0;
}