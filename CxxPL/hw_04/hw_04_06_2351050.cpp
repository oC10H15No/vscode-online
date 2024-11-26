/***
请使⽤泛型相关知识完成以下需求：
定义⼀个函数模版，计算出某些类型数据的最⼤值，如果取得的最⼤值的元素不唯⼀，则输出第⼀个取得最⼤值的元素。

Template<typename T>
T max(T t1, T t2, T t3){
……
}

需要计算的数据类型有：int, double, char, string, Rectangle 结构体（其中包含 x 和 y
两个整形变量，思考⼀下如何定义该结构体）, Rectangle*。
Ø int：例如 max(1，2，3)会输出 3，max(2，3，4)会输出 4
Ø double：与 int 类似，输出类型是 double（⽆需格式化）
Ø char：例如 max(’a’,’b’,’c’)会输出 c（ASCII 码⼤的更⼤）
Ø string：例如 max(string(“abc”)，string(“abcd”)，string(“abcde”))，输出 abcde（⻓度⻓
的更⼤）
Ø Rectangle：例如 max(Rectangle{1,2},Rectangle{2,6},Rectangle{3,4})，输出(2,6) (x,y 分别
代表⻓⽅形的⻓和宽，⾯积⼤的更⼤，(2,6)是第⼀个取得最⼤值⾯积 12 的元素)
Ø Rectangle*：与 Rectangle 输出类型⼀样，需要思考如何传参以及如何格式化输出

输⼊：按照顺序输⼊上述类型的三个数据，样例
1 2 3
1.3 2.4 4.6
a b d
csd swe sfdfg
1,2 3,4 2,6
1,2 2,6 3,4
第 5 ⾏的 1,2 3,4 2,6 是指三个结构体变量：(1,2)、(3,4)、(2,6)，思考如何输⼊。
输出：对应输⼊每⼀⾏输出⼀⾏结果（可以输⼊完⼀⾏，回⻋后直接输出，也可以数据全部
输⼊完之后输出），对应样例
3
4.6
d
sfdfg
3,4
2,6
 */

#include <iostream>
#include <string>
#include <type_traits>
#include <functional>

// 定义 Rectangle 结构体
struct Rectangle
{
    int width;
    int height;
    int area() const { return width * height; }
    // 重载 > 运算符
    bool operator>(const Rectangle& other) const { return this->area() > other.area(); }
    // 重载 << 运算符
    friend std::istream& operator>>(std::istream& is, Rectangle& r)
    {
        char comma;
        is >> r.width >> comma >> r.height;
        
        if (comma != ',' || r.width <= 0 || r.height <= 0) {
            std::cerr << "Invalid input" << std::endl;
            exit(1);
        }

        return is;
    }
    // 重载 << 运算符
    friend std::ostream& operator<<(std::ostream& os, const Rectangle& r)
    {
        os << "(" << r.width << ", " << r.height << ")";
        return os;
    }
};

template <typename T, typename Comp = std::function<bool(const T&, const T&)>>
T max(T t1, T t2, T t3, Comp comp = std::greater<T>())
{
    if (comp(t1, t2) && comp(t1, t3))
        return t1;
    else if (comp(t2, t1) && comp(t2, t3))
        return t2;
    else
        return t3;
}

// 定义 string 的比较函数
inline bool strComp(const std::string& a, const std::string& b) { return a.size() >= b.size(); }

template <>
std::string max(std::string t1, std::string t2, std::string t3, std::function<bool(const std::string&, const std::string&)> strComp)
{
    if (strComp(t1, t2) && strComp(t1, t3))
        return t1;
    else if (strComp(t2, t1) && strComp(t2, t3))
        return t2;
    else
        return t3;
}


// 定义 Rectangle 结构体的比较函数
inline bool recComp(const Rectangle& a, const Rectangle& b) { return a.area() >= b.area(); }

template <>
Rectangle max(Rectangle t1, Rectangle t2, Rectangle t3, std::function<bool(const Rectangle&, const Rectangle&)> recComp)
{
    if (recComp(t1, t2) && recComp(t1, t3))
        return t1;
    else if (recComp(t2, t1) && recComp(t2, t3))
        return t2;
    else
        return t3;
}

// 定义 Rectangle* 结构体指针的比较函数
inline bool recPtrComp(const Rectangle* a, const Rectangle* b) { return a->area() >= b->area(); }

template <>
Rectangle* max(Rectangle* t1, Rectangle* t2, Rectangle* t3, std::function<bool(const Rectangle*, const Rectangle*)> recPtrComp)
{
    if (recPtrComp(t1, t2) && recPtrComp(t1, t3))
        return t1;
    else if (recPtrComp(t2, t1) && recPtrComp(t2, t3))
        return t2;
    else
        return t3;
}

// 处理按类型的输入
template <typename T>
void handleInput()
{
    T t1, t2, t3;
    std::cout << "Enter three values devided by space: ";

    if constexpr (std::is_same_v<T, Rectangle*>) {
        // 指针方式输入
        t1 = new Rectangle;
        t2 = new Rectangle;
        t3 = new Rectangle;
        std::cin >> *t1 >> *t2 >> *t3;
    }
    else std::cin >> t1 >> t2 >> t3;
    
    if (!std::cin.fail()) {
        if constexpr (std::is_same_v<T, Rectangle*>) {
            std::cout << "max(" << *t1 << ", " << *t2 << ", " << *t3 << ") = " << *max(t1, t2, t3, recPtrComp) << std::endl;
            delete t1;
            delete t2;
            delete t3;
        }
        else if constexpr (std::is_same_v<T, Rectangle>) {
            std::cout << "max(" << t1 << ", " << t2 << ", " << t3 << ") = " << max(t1, t2, t3, recComp) << std::endl;
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << "max(" << t1 << ", " << t2 << ", " << t3 << ") = " << max(t1, t2, t3, strComp) << std::endl;
        }
        else
            std::cout << "max(" << t1 << ", " << t2 << ", " << t3 << ") = " << max(t1, t2, t3) << std::endl;
    }
    else {
        std::cerr << "Invalid input" << std::endl;
        exit(1);
    }
}

int main()
{
    //按类型依次测试
    try {
        std::cout << "1. Test with int" << std::endl;
        handleInput<int>();
        std::cout << "2. Test with double" << std::endl;
        handleInput<double>();
        std::cout << "3. Test with char" << std::endl;
        handleInput<char>();
        std::cout << "4. Test with string" << std::endl;
        handleInput<std::string>();
        std::cout << "5. Test with Rectangle" << std::endl;
        handleInput<Rectangle>();
        std::cout << "6. Test with Rectangle*" << std::endl;
        handleInput<Rectangle*>();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}