#include <iostream>
#include <string>
#include <cstring>
#include <type_traits>
#include <sstream>
#include <functional>

struct Rectangle
{
    int width;
    int height;
    int area() const { return width * height; }
    bool operator>(const Rectangle& other) const { return this->area() > other.area(); }
    friend std::istream& operator>>(std::istream& is, Rectangle& r)
    {
        char comma;
        is >> r.width >> comma >> r.height;
        
        if (comma != ',') {
            is.setstate(std::ios_base::failbit);
            throw std::invalid_argument("Invalid input format");
        }

        return is;
    }
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

template <typename T>
void handleInput()
{
    T v1, v2, v3;
    std::cout << "Enter three values devided by space: ";
    if constexpr (std::is_same_v<T, Rectangle*>) {
        Rectangle v1, v2, v3;
        Rectangle* ptrRec1 = &v1;
        Rectangle* ptrRec2 = &v2;
        Rectangle* ptrRec3 = &v3; 
        std::cin >> *ptrRec1 >> *ptrRec2 >> *ptrRec3;
    }
    else std::cin >> v1 >> v2 >> v3;
    
    if (!std::cin.fail()) {
        if constexpr (std::is_same_v<T, Rectangle>) {
            std::cout << "max(" << v1 << ", " << v2 << ", " << v3 << ") = " << max(v1, v2, v3, recComp) << std::endl;
        }
        else if constexpr (std::is_same_v<T, Rectangle*>) {
            std::cout << "max(" << *v1 << ", " << *v2 << ", " << *v3 << ") = " << *max(v1, v2, v3, recPtrComp) << std::endl;
        }
        else
            std::cout << "max(" << v1 << ", " << v2 << ", " << v3 << ") = " << max(v1, v2, v3) << std::endl;
    }
    else {
        throw std::invalid_argument("Invalid input");
    }
}

int main()
{
    /*
    std::cout << "1. Test with int" << std::endl;
    try {
        handleInput<int>();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "2. Test with double" << std::endl;
    try {
        handleInput<double>();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "3. Test with char" << std::endl;
    try {
        handleInput<char>();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "4. Test with string" << std::endl;
    try {
        handleInput<std::string>();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
    */
    std::cout << "5. Test with Rectangle" << std::endl;
    try {
        handleInput<Rectangle>();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "6. Test with Rectangle*" << std::endl;
    try {
        handleInput<Rectangle*>();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
    

    

    return 0;
}
