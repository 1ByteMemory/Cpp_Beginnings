// Cpp_Beginings.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std; // std has things like string

void Print(string);


int main()
{
    Print("Hello World");
    Print("You beurtiful princess!");
    
}


void Print(string output)
{
    cout << output << endl;
}