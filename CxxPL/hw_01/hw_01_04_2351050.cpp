/*
Problem 4:
Create a struct that holds two string objects and one int. Use a typedef for the struct
name. Create an instance of the struct, initialize all three values in your instance, and print
them out. Take the address of your instance and assign it to a pointer to your struct type.
Change the three values in your instance and print them out, all using the pointer.
*/

#include <iostream>
#include <string>

using namespace std;

struct MyStruct {
    string str1;
    string str2;
    int num;
};

typedef MyStruct defMyStruct;

int main()
{
    // Create and initialize the struct instance.
    defMyStruct instance = {"1st string","2nd string",1907};
    // Print initial values.
    cout << "Initial values: " << instance.str1 << ", " << instance.str2 
        << ", " << instance.num << endl;
    
    defMyStruct* pinstance = &instance;
    // Modify values through the pointer.
    pinstance->str1 = "Modified 1st string";
    pinstance->str2 = "Modified 2nd string";
    pinstance->num = 65472;

    cout << "Modified values via pointer: " << pinstance->str1 << ", " << pinstance->str2 
        << ", " << pinstance->num << endl;

    return 0;
}