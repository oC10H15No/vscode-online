#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

/*
Problem 2:
Create a vector<float> and put 25 numbers into it. Then square each number and put
the result back into the same location in the vector. Display the vector before and after the
multiplications.
*/

const int NUM=25;

void PrintVector(const vector<float>& vec, const string& prompt)
{
    cout << prompt << ": ";
    for (int i=0 ; i<NUM; i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

int main() 
{
    vector<float> numbers;

    //Initialize
    for (int i=0; i<NUM; i++) {
        numbers.push_back(static_cast<float>(i+1));
    }

    PrintVector(numbers, "Original numbers");

    //Square
    for (int i=0; i<NUM; i++) {
        numbers[i] = pow(numbers[i], 2);
    }

    PrintVector(numbers, "Squared numbers");

    return 0;
}