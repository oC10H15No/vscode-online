/* 2351050 杨瑞晨 */
#include <iostream>
#include <string>
using namespace std;

// 判断字符串s在[l, r]区间是否为回文串
bool isPalindrome(const string &s, int l, int r) {
    while (l < r) {
        if (s[l] != s[r]) {
            return false;
        }
        l++;
        r--;
    }
    return true;
}

// 判断删除一个字符后是否为回文串
bool deletePalindrome(const string &s) {
    int l = 0, r = s.length() - 1;

    while (l < r) {
        if (s[l] != s[r]) {
            // 尝试跳过左边或者右边的字符，看看是否成为回文串
            return isPalindrome(s, l + 1, r) || isPalindrome(s, l, r - 1);
        }
        l++;
        r--;
    }

    // 如果原串本身就是回文串，则直接返回true
    return true;
}

int main() {
    string input;
    cout << "Please input the brand of the chocolate: ";
    cin >> input;

    if (deletePalindrome(input)) {
        cout << "True" << endl;
    } 
    else {
        cout << "False" << endl;
    }

    return 0;
}