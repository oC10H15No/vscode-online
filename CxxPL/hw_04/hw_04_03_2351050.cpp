/***
 * Create two classes called Traveler and Pager without default constructors, but with
constructors that take an argument of type string, which they simply copy to an internal string
variable. For each class, write the correct copy-constructor and assignment operator. Now
inherit a class BusinessTraveler from Traveler and give it a member object of type Pager. Write
the correct default constructor, a constructor that takes a string argument, a copy-constructor,
and an assignment operator.
 */

#include <iostream>
#include <string>

class Traveler {
private:
    std::string m_name;
public:
    // constructor takes an argument of type string
    Traveler(const std::string& name) : m_name(name) {}
    // copy constructor
    Traveler(const Traveler& other) : m_name(other.m_name) {}
    // assignment operator
    Traveler& operator=(const Traveler& other) {
        if (this != &other) {
            m_name = other.m_name;
        }
        return *this;
    }
    std::string getName() const { return m_name; }
};

class Pager {
private:
    std::string m_number;
public:
    // constructor takes an argument of type string
    Pager(const std::string& number) : m_number(number) {}
    // copy constructor
    Pager(const Pager& other) : m_number(other.m_number) {}
    // assignment operator
    Pager& operator=(const Pager& other) {
        if (this != &other) {
            m_number = other.m_number;
        }
        return *this;
    }
    std::string getNumber() const { return m_number; }
};

class BusinessTraveler : public Traveler {
private:
    Pager m_pager;
public:
    // default constructor
    BusinessTraveler() : Traveler("Default traveler"), m_pager("0") {}
    // constructor takes string arguments
    BusinessTraveler(const std::string& name, const std::string& device) : Traveler(name), m_pager(device) {}
    // copy constructor
    BusinessTraveler(const BusinessTraveler& other) : Traveler(other), m_pager(other.m_pager) {}
    // assignment operator
    BusinessTraveler& operator=(const BusinessTraveler& other) {
        if (this != &other) {
            Traveler::operator=(other);
            m_pager = other.m_pager;
        }
        return *this;
    }
    std::string getPagerNumber() const { return m_pager.getNumber(); }
};

int main() {
    // 创建 BusinessTraveler 对象
    BusinessTraveler bt1("Alice", "123456");
    std::cout << "bt1 Name: " << bt1.getName() << ", Pager: " << bt1.getPagerNumber() << std::endl;
    // 使用拷贝构造函数
    BusinessTraveler bt2(bt1);
    std::cout << "bt2 Name: " << bt2.getName() << ", Pager: " << bt2.getPagerNumber() << std::endl;
    // 使用默认构造函数和赋值运算符
    BusinessTraveler bt3;
    std::cout << "bt3 Name: " << bt3.getName() << ", Pager: " << bt3.getPagerNumber() << std::endl;
    bt3 = bt1;
    std::cout << "bt3 Name: " << bt3.getName() << ", Pager: " << bt3.getPagerNumber() << std::endl;

    // 修改 bt1 的值，验证 bt2 和 bt3 是否独立
    bt1 = BusinessTraveler("Bob", "654321");
    std::cout << "After modification: " << std::endl;
    std::cout << "bt1 Name: " << bt1.getName() << ", Pager: " << bt1.getPagerNumber() << std::endl;
    std::cout << "bt2 Name: " << bt2.getName() << ", Pager: " << bt2.getPagerNumber() << std::endl;
    std::cout << "bt3 Name: " << bt3.getName() << ", Pager: " << bt3.getPagerNumber() << std::endl;

    // 验证继承关系
    Traveler* travelerPtr = &bt1;
    std::cout << "Traveler pointer to bt1 Name: " << travelerPtr->getName() << std::endl;
    
    return 0;
}