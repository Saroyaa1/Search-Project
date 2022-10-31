//*Project 2 cs251.cpp*/

// This file takes in a simple file from the main fuction in the cpp file
// and send it to searchEngine() to form a inverted index the user can search
// through. 
//
// Anant Raj Saroya
// Ned id: 671783409
// U. of Illinois, Chicago

#include <iostream>
#include "search.h"
#include <string>

using namespace std;

int main() {

    string filename = "cplusplus.txt";
    searchEngine(filename);

    return 0;
        
}
