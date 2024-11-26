#include <iostream>
#include <vector>
#include <algorithm>  // For min and max

using namespace std;

/*
Problem 5
给定一个数组 prices ，它的第 i 个元素 prices[i] 表示一支给定股票第 i 天的价格。
你只能选择 某一天 买入这只股票，并选择在 未来的某一个不同的日子 卖出该股票。
设计一个算法来计算你所能获取的最大利润。
返回你可以从这笔交易中获取的最大利润。如果你不能获取任何利润，返回 0 。
*/

// Function to read prices
vector<int> get_prices()
{
    vector<int> prices;
    int price = 0;

    cout << "Please input stock prices seperated by space and ended with -1: ";
    while (true) {
        cin >> price;
        if (price < -1  || cin.fail()) {
            cerr << "Invalid input!" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            return get_prices();
        }
        else if (price == -1) {
            break;
        }
        else {
            prices.push_back(price);
            continue;
        }
    }
    return prices;
}

// Function to calculate the maximum profit
int max_profit(const vector<int> &prices)
{
    if (prices.empty()) {
        return 0;
    }

    int min_price = prices[0];
    int max_profit = 0;

    for(int i=1; i<prices.size(); i++) {
        // Update the minimum price encountered so far
        min_price = min(min_price, prices[i]);
        // Calculate potential profit at the current price and update max_profit
        max_profit = max(max_profit, prices[i] - min_price);
    }

    return max_profit;
}

int main()
{
    vector<int> prices = get_prices();

    int profit = max_profit(prices);
    cout << "The maximum profit you can get is " << profit << "." << endl;

    return 0;
}