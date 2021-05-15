// Cpp_Beginings.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>


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


// Multi dimensional arrays
int arr2D[2][3] = { {10, 11, 12}, {20, 21, 22} };
int arr3d[2][3][2] = {
    { {1,-1}, {2, -2}, {3,-1} },
    { {4,-4}, {5, -5}, {6,-6} }
};


/*
int main()
{
    //cout << f(5) << endl;

    // User Input
    string str;
    cout << "Enter a string: ";
    
    getline(cin, str); // <string>
    cout << "You entered: ";
    cout << str << endl;

    // This function add a character at the end of the string
    str.push_back('A');
    cout << "The string after push back: " << str << endl;

    str.pop_back();
    cout << "the string after pop_back: " << str << endl;

    return 0;
}
*/