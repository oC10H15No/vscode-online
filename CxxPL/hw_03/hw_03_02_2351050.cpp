/***
2. 存在重复元素 II
给你⼀个整数数组 nums 和⼀个整数 k ，判断数组中是否存在两个不同的索引 i 和 j ，使得
nums[i] == nums[j] 并且 abs(i - j) <= k 。
示例 1
输⼊: nums = [1,2,3,1], k = 3
输出: true
示例 2 :
输⼊: nums = [1,0,1,1], k = 1
输出: true
示例 3 :
输⼊: nums = [1,2,3,1,2,3], k = 2
输出: false提示 :
1 <= nums.length <= 10^5
-10^9 <= nums[i] <= 10^9
0 <= k <= 10^5
***/

#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <sstream>

/**
 * function name:            containsNearbyDuplicate
 * function description:     判断整数数组中是否存在相近元素，（不传入 valueDiff 时即判断是否存在重复元素）且索引差值小于等于 indexDiff
 * input parameters:         std::vector<int> &nums 整数数组的引用
 * return values:            bool值  true: 存在相近元素 且索引差值小于等于 indexDiff，值差小于等于 valueDiff
 *                                false: 不存在相近元素 或索引差值大于 indexDiff 或 值差大于 valueDiff
 */
bool containsNearbyDuplicate(const std::vector<int> &nums, int indexDiff = 10e5, int valueDiff = 0)
{
    std::map<int, int> numMap; // 用于存储元素及其索引 <数值, 索引值>

    for (int i = 0; i < nums.size(); ++i)
    {
        // 检查当前元素是否在 map 中存在满足条件的元素 即 [nums[i] - valueDiff, nums[i] + valueDiff]
        auto it = numMap.lower_bound(nums[i] - valueDiff);             // 返回第一个大于等于 nums[i] - valueDiff 的元素的迭代器
        while (it != numMap.end() && it->first <= nums[i] + valueDiff) // 遍历 map 中所有满足条件的元素
        {
            if (abs(it->second - i) <= indexDiff)
            {
                return true;
            }
            ++it;
        }

        // 将当前元素及其索引添加到 map
        numMap[nums[i]] = i;
    }

    return false;
}

/**
 * function name:            readNums
 * function description:     从标准输入读取整数数组
 * input parameters:         None
 * return values:            std::vector<int> 读取的整数数组
 */
std::vector<int> readNums()
{
    std::cout << "Please input an integer array devided by space : ";

    std::vector<int> nums; // 用于存储整数的向量
    int num;               // 临时变量，用于存储每次读取的整数

    std::string line;             // 用于存储用户输入的一整行
    std::getline(std::cin, line); // 从标准输入读取一整行
    std::istringstream iss(line); // 使用 istringstream 将字符串转换为输入流

    // 循环读取输入流中的整数
    while (true)
    {
        iss >> num;     // 尝试从输入流中读取一个整数
        if (iss.fail()) // 如果读取失败
        {
            std::cerr << "Error: Invalid input" << std::endl;              // 输出错误信息
            iss.clear();                                                   // 清除错误状态
            iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 忽略输入流中的剩余内容
            nums.clear();                                                  // 清空已读取的整数
            exit(1);                                                       // 退出程序
        }
        else
        {
            nums.push_back(num); // 将读取的整数添加到向量中
        }

        // 如果到达输入流的末尾，退出循环
        if (iss.eof())
            break;
    }

    if (nums.size() <= 0)
    {
        std::cerr << "Error: Empty input" << std::endl;                // 输出错误信息
        iss.clear();                                                   // 清除错误状态
        iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 忽略输入流中的剩余内容
        nums.clear();                                                  // 清空已读取的整数
        exit(1);                                                       // 退出程序
    }

    return nums; // 返回读取的整数向量
}

int main()
{
    std::vector<int> nums = readNums();

    int k;
    std::cout << "PLease input the integer k : ";
    while (true)
    {
        std::cin >> k;
        if (k < 0 || std::cin.fail())
        {
            std::cerr << "Error: Invalid input" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            exit(1);
        }
        else break;
    }

    std::cout << (containsNearbyDuplicate(nums, k) ? "true" : "false") << std::endl;
    
    return 0;
}