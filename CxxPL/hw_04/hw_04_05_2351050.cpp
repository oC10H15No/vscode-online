/***
 * Create a base class containing a clone() function that returns a pointer to a copy of the
current object. Derive two subclasses that override clone() to return copies of their specific
types. In main(), create and upcast objects of your two derived types, then call clone() for each
and verify that the cloned copies are the correct subtypes. Experiment with your clone()
function so that you return the base type, then try returning the exact derived type. Can you
think of situations in which the latter approach is necessary?
 */

#include <iostream>
#include <typeinfo>

class Base {
public:
    virtual Base* clone() const = 0;
    virtual ~Base() = default;
};

class Derived1 : public Base {
public:
    Derived1* clone() const { return new Derived1(*this); }
};

class Derived2 : public Base {
public:
    Derived2* clone() const { return new Derived2(*this); }
};

int main()
{
    // Upcast objects of derived types to base type
    Base* basePtr1 = new Derived1();
    Base* basePtr2 = new Derived2();

    // Call clone() for each and verify that the cloned copies are the correct subtypes
    Base* clonedBasePtr1 = basePtr1->clone();
    Base* clonedBasePtr2 = basePtr2->clone();

    std::cout << "basePtr1 is of type " << typeid(*basePtr1).name() << std::endl;
    std::cout << "clonedBasePtr1 is of type " << typeid(*clonedBasePtr1).name() << std::endl;
    std::cout << "basePtr2 is of type " << typeid(*basePtr2).name() << std::endl;
    std::cout << "clonedBasePtr2 is of type " << typeid(*clonedBasePtr2).name() << std::endl;

    delete basePtr1;
    delete basePtr2;
    delete clonedBasePtr1;
    delete clonedBasePtr2;

    return 0;
}