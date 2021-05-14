// Cpp_Beginings.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std; // std has things like string

// Recursive functions
int f(int n)
{
    // Base function to stop never ending calling
    if (n <= 1)
    {
        return 1;
    }
    else
    {
        return n * f(n - 1);
    }
}


int main()
{
    cout << f(5) << endl;
    return 0;
}