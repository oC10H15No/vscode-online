/***
 * Write a class with one virtual function and one non-virtual function. Inherit a new class,
make an object of this class, and upcast to a pointer of the base-class type. Use the clock()
function found in <ctime>(you’ll need to look this up in your local C library guide) to measure
the difference between a virtual call and non-virtual call. You’ll need to make multiple calls to
each function inside your timing loop to see the difference.
 */

#include <iostream> 
#include <ctime>

class Base {
public:
    virtual void virtualFunction() { std::cout << "Base::virtualFunction()" << std::endl; }
    void nonVirtualFunction() { std::cout << "Base::nonVirtualFunction()" << std::endl; }
};

class Derived : public Base {
public:
    void virtualFunction() { std::cout << "Derived::virtualFunction()" << std::endl; }
    void nonVirtualFunction() { std::cout << "Derived::nonVirtualFunction()" << std::endl; }
};

int main() {
    const int iterations = 1000000;

    Base* basePtr = new Derived();

    // Measure the time for virtual function call
    clock_t start = clock();
    for (int i = 0; i < iterations; i++) {
        basePtr->virtualFunction();
    }
    clock_t end = clock();

    // Measure the time for non-virtual function call
    clock_t start2 = clock();
    for (int i = 0; i < iterations; i++) {
        basePtr->nonVirtualFunction();
    }
    clock_t end2 = clock();

    std::cout << "Time for virtual function call: " << static_cast<double>(end - start) / CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << "Time for non-virtual function call: " << static_cast<double>(end2 - start2) / CLOCKS_PER_SEC << "s" << std::endl;

    delete basePtr;

    return 0;
}