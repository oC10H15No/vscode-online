#include <iostream>
#include <vector>
#include <limits>
#include <stdio.h>
#include <array>
#include <unordered_map>
#include <optional>
#include <future>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;

/**
 * 1.正确性：正确
 * 2.实现特点：这种实现方式使用递归来计算斐波那契数列。
 *  **优点**：代码简洁，易于理解，适合教学和理解递归的基本概念。
 *  **缺点**：效率低，时间复杂度为 O(2^n)，对于较大的 n 会导致栈溢出。
 * 3.语法细节：没有特别复杂的语法或算法细节。
 * 4.适用场景：适合教学和理解递归原理，或对性能要求不高的小规模问题。
 */

/// @brief 递归计算斐波那契数列
/// @param n 斐波那契数列的第 n 项
/// @return 第 n 项的值
int fibonacci_recursive(int n)
{
    if (n <= 1)
        return n;
    return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2); // 递归调用 F(n) = F(n-1) + F(n-2)
}

/**
 * 1.正确性：正确
 * 2.实现特点：这种实现方式使用迭代来计算斐波那契数列。
 *  **优点**：效率高，时间复杂度为 O(n)，适合需要快速计算的场景。
 *  **缺点**：代码相对递归实现稍微复杂一些。
 * 3.语法细节：没有特别复杂的语法或算法细节。
 * 4.适用场景：适合需要快速计算和节省内存的场景。
 */

/// @brief 迭代计算斐波那契数列
/// @param n  斐波那契数列的第 n 项
/// @return  第 n 项的值
int fibonacci_iterative(int n)
{
    if (n <= 1)
        return n;
    int a = 0, b = 1, temp;
    for (int i = 2; i <= n; ++i)
    {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

/**
 * 1.正确性：正确
 * 2.实现特点：这种实现方式使用动态规划来计算斐波那契数列。
 *  **优点**：效率高，时间复杂度为 O(n)，避免了递归的栈溢出问题而且计算每一项只依赖前两项，避免了大量的重复计算。。
 *  **缺点**：需要额外的空间来存储中间结果。
 * 3.语法细节：没有特别复杂的语法或算法细节。
 * 4.适用场景：适合中等规模问题，能够很好地在递归与空间效率间取得平衡。
 */

/// @brief  动态规划计算斐波那契数列
/// @param n  斐波那契数列的第 n 项
/// @return  第 n 项的值
int fibonacci_dp(int n)
{
    if (n <= 1)
        return n;
    std::vector<int> dp(n + 1, 0);
    dp[1] = 1;
    for (int i = 2; i <= n; ++i)
    {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}

/**
 * 1.正确性：正确
 * 2.实现特点：这种实现方式使用矩阵快速幂来计算斐波那契数列。
 *  **优点**：时间复杂度为 O(log n)，适合需要快速计算大数的场景。
 *  **缺点**：实现复杂，理解难度较高。
 * 3.语法细节：矩阵乘法和快速幂的实现。
 * 斐波那契数列关系可用矩阵表示：
 * | F(n)   |   | 1 1 |   | F(n-1) |
 * | F(n-1) | = | 1 0 | * | F(n-2) |
 * 通过对这个 2x2 矩阵进行快速幂运算，就可以高效地计算 F(n)。
 * 快速幂原理
 * 快速幂通过将指数 n 用二进制表示并根据二进制的每一位进行幂次的累积来减少运算次数。
 * 例如，计算 base^n，我们可以按以下方式逐步累积：
 * 如果 n 的二进制表示的某一位是 1，我们将当前的 base 值乘入结果。
 * 无论这一位是否为 1，我们都会将 base 自乘（幂次倍增）。
 * 4.适用场景：适合大规模问题，尤其是当 n 非常大时。
 */

/// @brief  矩阵乘法
/// @param a  2x2 矩阵 a
/// @param b  2x2 矩阵 b
/// @return  矩阵 a 和 b 的乘积
std::array<std::array<long long, 2>, 2> matrix_multiply(const std::array<std::array<long long, 2>, 2> &a,
                                                        const std::array<std::array<long long, 2>, 2> &b)
{
    std::array<std::array<long long, 2>, 2> result = {{0}};
    // 进行矩阵乘法运算：result[i][j] = Σ (a[i][k] * b[k][j])
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
                result[i][j] += a[i][k] * b[k][j]; // 矩阵乘法
    return result;
}

/// @brief  矩阵快速幂计算斐波那契数列
/// @param n  斐波那契数列的第 n 项
/// @return  第 n 项的值
long long fibonacci_matrix(int n)
{
    if (n <= 1) // 边界条件：当n为0或1时，直接返回n（F(0)=0，F(1)=1）
        return n;
    // 矩阵快速幂

    // 定义基本的转换矩阵 base = {{1, 1}, {1, 0}}，它能将 F(n) 和 F(n-1) 转换为 F(n+1) 和 F(n)
    std::array<std::array<long long, 2>, 2> base = {{{1, 1}, {1, 0}}};
    // 定义单位矩阵 result = {{1, 0}, {0, 1}}，它是矩阵乘法中的"1"
    std::array<std::array<long long, 2>, 2> result = {{{1, 0}, {0, 1}}};
    n--; // 从 F(1) 开始计算

    while (n > 0)
    {
        if (n & 1)                                  // 位运算：判断当前 n 的二进制最低位是否为 1
            result = matrix_multiply(result, base); // 如果最低位是1，则乘以当前 base
        base = matrix_multiply(base, base);         // base 自乘，进行幂次的二次方递推
        n >>= 1;                                    // 右移一位，相当于将 n 除以 2，用于处理更高位

    } // 返回结果矩阵中的 result[0][0]，这个元素存储的是 F(n)
    return result[0][0];
}

/**
 * 1.正确性：正确
 * 2.实现特点：这种实现方式使用记忆化递归来计算斐波那契数列。
 *  **优点**：避免了重复计算，时间复杂度为 O(n)，适合需要递归实现但又希望提高效率的场景。
 *  **缺点**：需要额外的空间来存储中间结果。
 * 3.语法细节：使用 std::unordered_map 进行记忆化。
 * std::unordered_map` 是一个哈希表实现，提供了快速的查找、插入和删除操作。
 * 使用 std::unordered_map 可以高效地存储和查找已经计算过的斐波那契数列的值，从而避免重复计算，提高算法的效率。
 * 4.适用场景：适合中等规模问题，特别是递归解决方案需要优化时。
 */

/// @brief  记忆化递归计算斐波那契数列
/// @param n  斐波那契数列的第 n 项
/// @param memo  记忆化的中间结果
/// @return  第 n 项的值
long long fibonacci_memoization(int n, std::unordered_map<int, long long> &memo)
{
    if (n <= 1)
        return n;
    // 检查是否已经计算过第 n 项，若存在则直接返回已缓存的结果
    if (memo.find(n) != memo.end())
        return memo[n];
    // 否则递归计算第 n-1 和 n-2 项，并将结果保存到 memo 以避免重复计算
    memo[n] = fibonacci_memoization(n - 1, memo) +
              fibonacci_memoization(n - 2, memo);
    return memo[n];
}

/**
 * 1.正确性：正确
 * 2.实现特点：这种实现方式使用 std::optional 来处理负数输入。
 *  **优点**：可以安全地处理负数输入，适合需要处理异常输入的场景。
 *  **缺点**：增加了代码复杂度。
 * 3.语法细节：使用 std::optional 来处理可能的无效输入。
 * std::optional 是 C++17 引入的一个模板类，用于表示一个值可能存在也可能不存在。
 * 它可以安全地处理可能的无效输入，避免返回特殊值（如 -1 或 NULL）来表示无效输入，从而提高代码的可读性和安全性。
 * 4.适用场景：适合需要处理异常输入的情况。
 */

/// @brief  安全计算斐波那契数列
/// @param n  斐波那契数列的第 n 项
/// @return  第 n 项的值，如果 n 为负数则返回 std::nullopt
std::optional<long long> fibonacci_safe(int n)
{
    // 处理负数输入，返回 std::nullopt 表示无效输入
    if (n < 0)
        return std::nullopt;
    if (n <= 1)
        return n;
    long long a = 0, b = 1, temp;
    for (int i = 2; i <= n; ++i)
    {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

/**
 * 1.正确性：正确
 * 2.实现特点：这种实现方式使用 constexpr 来计算斐波那契数列。
 *  **优点**：可以在编译时计算常量表达式，适合需要在编译时确定结果的场景
 *  **缺点**：只能用于编译时常量计算，不能处理运行时输入。
 * 3.语法细节：使用 constexpr 进行编译时计算。
 * constexpr 是 C++11 引入的一个关键字，用于指定一个表达式或函数可以在编译时求值。
 * 使用 constexpr 可以在编译时计算常量表达式，从而提高程序的性能和安全性。
 * 4.适用场景：适合在编译时确定结果的场景，如常量表达式计算。
 */

/// @brief  编译时计算斐波那契数列
/// @param n  斐波那契数列的第 n 项
/// @return  第 n 项的值
constexpr long long fibonacci_constexpr(int n)
{
    if (n <= 1)
        return n;
    long long a = 0, b = 1;
    for (int i = 2; i <= n; ++i)
    {
        long long temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

/**
 * 1.正确性：正确
 * 2.实现特点：这种实现方式使用 std::async 并行计算来计算斐波那契数列。
 *  **优点**：可以利用多核处理器提高计算速度，适合需要并行计算的场景。
 *  **缺点**：实现复杂，线程管理开销较大。
 * 3.语法细节：使用 std::async 进行并行计算。
 * std::async 是 C++11 引入的一个模板函数，用于启动异步任务。
 * 它会启动一个新的线程来执行指定的函数，并返回一个 std::future 对象，用于获取异步任务的结果。
 * 4.适用场景：适合需要并行加速的场景，但对于 Fibonacci 问题，开销较大，通常不推荐。
 */

/// @brief  并行计算斐波那契数列
/// @param n  斐波那契数列的第 n 项
/// @return  第 n 项的值
long long fibonacci_parallel(int n)
{
    if (n <= 1)
        return n;
    auto future = std::async(std::launch::async, fibonacci_parallel, n - 2);
    long long result = fibonacci_parallel(n - 1);
    return result + future.get();
}

/**
 * 1.正确性：正确
 * 2.实现特点：这种实现方式使用 Boost 库中的大整数类型来计算斐波那契数列。
 *  **优点**：可以处理非常大的数，适合需要高精度计算的场景。
 *  **缺点**：需要依赖 Boost 库，增加了外部依赖。
 * 3.语法细节：使用 Boost 库中的大整数类型。
 * Boost 库是一个 C++ 库，提供了许多功能强大的工具和组件，包括大整数类型、多线程、图形库等。
 * 4.适用场景：适合高精度计算场景，尤其是 n 很大时。
 */

/// @brief  使用 Boost 库中的大整数类型计算斐波那契数列
/// @param n  斐波那契数列的第 n 项
/// @return  第 n 项的值 (大整数类型)
boost::multiprecision::cpp_int fibonacci_bigint(int n)
{
    if (n <= 1)
        return n;
    // 使用 Boost 库的大整数类型进行斐波那契数列计算
    boost::multiprecision::cpp_int a = 0, b = 1, temp;
    for (int i = 2; i <= n; ++i)
    {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

/**
 * 1.正确性：正确
 * 2.实现特点：这种实现方式使用模板元编程来计算斐波那契数列。
 *  **优点**：可以在编译时计算常量表达式，适合需要在编译时确定结果的场景。
 *  **缺点**：只能用于编译时常量计算，不能处理运行时输入。
 * 3.语法细节：使用模板元编程进行编译时计算。
 * 4.适用场景：适合需要在编译时计算 Fibonacci 数列的场景。
 */

/// @brief  使用模板元编程在编译时计算斐波那契数列的第 N 项
/// @tparam N  斐波那契数列的第 N 项，必须为编译时常量
template <int N, typename = std::enable_if_t<(N >= 0)>>
struct Fibonacci
{
    static constexpr long long value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template <>
struct Fibonacci<0>
{
    static constexpr long long value = 0;
};

template <>
struct Fibonacci<1>
{
    static constexpr long long value = 1;
};

std::optional<long long> fibonacci(int n);

/// @brief  主函数
/// @return  执行结果
int main()
{
    const int times = 15;
    cout << "1:" << endl;
    for (int i = 0; i <= times; i++)
    {
        cout << "Fib(" << i << ")= " << fibonacci_recursive(i) << endl;
    }
    cout << endl;
    getchar();
    cout << "2:" << endl;
    for (int i = 0; i <= times; i++)
    {
        cout << "Fib(" << i << ")= " << fibonacci_iterative(i) << endl;
    }
    cout << endl;
    getchar();
    cout << "3:" << endl;
    for (int i = 0; i <= times; i++)
    {
        cout << "Fib(" << i << ")= " << fibonacci_dp(i) << endl;
    }
    cout << endl;
    getchar();
    cout << "4:" << endl;
    for (int i = 0; i <= times; i++)
    {
        cout << "Fib(" << i << ")= " << fibonacci_matrix(i) << endl;
    }
    cout << endl;
    getchar();
    cout << "5:" << endl;
    for (int i = 0; i <= times; i++)
    {
        std::unordered_map<int, long long> memo;
        cout << "Fib(" << i << ")= " << fibonacci_memoization(i, memo) << endl;
    }
    cout << endl;
    getchar();
    cout << "6:" << endl;
    for (int i = 0; i <= times; i++)
    {
        auto result = fibonacci_safe(i);
        if (result)
            cout << "Fib(" << i << ")= " << *result << endl;
        else
            cout << "Fib(" << i << ")= " << "Invalid input" << endl;
    }
    cout << endl;
    getchar();
    cout << "7:" << endl;
    for (int i = 0; i <= times; i++)
    {
        cout << "Fib(" << i << ")= " << fibonacci_constexpr(i) << endl;
    }
    cout << endl;
    getchar();
    cout << "8:" << endl;
    for (int i = 0; i <= times; i++)
    {
        cout << "Fib(" << i << ")= " << fibonacci_parallel(i) << endl;
    }
    cout << endl;
    getchar();
    cout << "9:" << endl;
    for (int i = 0; i <= times; i++)
    {
        cout << "Fib(" << i << ")= " << fibonacci_bigint(i) << endl;
    }
    cout << endl;
    getchar();
    cout << "10:" << endl;
    {
        cout << "Fib(1)= " << Fibonacci<1>::value << endl;
        cout << "Fib(2)= " << Fibonacci<2>::value << endl;
        cout << "Fib(3)= " << Fibonacci<3>::value << endl;
        cout << "Fib(4)= " << Fibonacci<4>::value << endl;
        cout << "Fib(5)= " << Fibonacci<5>::value << endl;
        cout << "Fib(6)= " << Fibonacci<6>::value << endl;
        cout << "Fib(7)= " << Fibonacci<7>::value << endl;
        cout << "Fib(8)= " << Fibonacci<8>::value << endl;
        cout << "Fib(9)= " << Fibonacci<9>::value << endl;
        cout << "Fib(10)= " << Fibonacci<10>::value << endl;
        cout << "Fib(11)= " << Fibonacci<11>::value << endl;
        cout << "Fib(12)= " << Fibonacci<12>::value << endl;
        cout << "Fib(13)= " << Fibonacci<13>::value << endl;
        cout << "Fib(14)= " << Fibonacci<14>::value << endl;
        cout << "Fib(15)= " << Fibonacci<15>::value << endl;
    }
    getchar();
    cout << "MyFib:" << endl;
    for (int i = 0; i <= times; i++)
    {
        auto result = fibonacci(i);
        if (result)
            cout << "Fib(" << i << ")= " << *result << endl;
        else
            cout << "Fib(" << i << ")= " << "Invalid input" << endl;
    }
    cout << endl;
    return 0;
}

/** my_fib: 结合动态规划和 std::optional 与 constexpr 实现的斐波那契数列计算
 * 从时间复杂度来说矩阵快速幂是最优的 O(n)，但是在实际应用中，由于矩阵快速幂的实现较为复杂，
 * 且对于斐波那契数列的计算来说，通常不会运算过大的数字，动态规划的时间复杂度 O(n) 已经足够.
 * -动态规划：fibonacci 函数使用动态规划来计算斐波那契数列的第 n 项，
 * -使用 std::optional 来处理负数输入。如果 `n` 为负数，返回 `std::nullopt`，表示无效输入。
 * - **优点**：
 * - 动态规划方法具有高效的时间复杂度 O(n)。
 * - 使用 `std::optional` 可以安全地处理无效输入。
 * - **缺点**：
 * - 动态规划方法需要额外的空间来存储中间结果。
 */

/// @brief  动态规划计算斐波那契数列
/// @param n 斐波那契数列的第 n 项
/// @return 第 n 项的值，如果 n 为负数则返回 std::nullopt
std::optional<long long> fibonacci(int n)
{
    if (n < 0)
        return std::nullopt;
    if (n <= 1)
        return n;
    vector<long long> dp(n + 1, 0);
    dp[1] = 1;
    for (int i = 2; i <= n; ++i)
    {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}