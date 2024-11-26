/*
Problem 3:
Create two functions, one that takes a string* and one that takes a string&. Each of these
functions should modify the outside string object in its own unique way. In main(), create and
initialize a string object, print it, then pass it to each of the two functions, printing the results.
*/

#include <iostream>
#include <string>

using namespace std;

void ModifyByPointer(string* str) 
{
  *str += " (modified by pointer)";
}

void ModifyByReference(string& str) 
{
  str += " (modified by reference)";
}

int main() 
{
  string str = "Original string";
  cout << "Initial string: " << str << endl;

  ModifyByPointer(&str);
  cout << "String after modified by pointer: " << str << endl;

  ModifyByReference(str);
  cout << "String after modified by reference: " << str << endl;

  return 0;
}