/*

Program 2: Search Engine
Course: CS 251
Author: Daniel Luangnikone dluan3@uic.edu
System: Windows 11, using VSC

This program runs a search engine from user input and finds matches

*/

#include <iostream>
#include "search.h"

using namespace std;

int main() 
{

    // map<string, set<string>> index;

    // cout << cleanToken("Hammerman") << endl;
    // gatherTokens("To be or not to be ");
    // buildIndex("cplusplus.txt", index);
    // findQueryMatches(index, "vector bool ratio");

    searchEngine("cplusplus.txt");

    // creative component;
    // creative function, takes a list of stop words and loops thru the index. See if index contains a stopword from the list, then removes it from index.
    // to use this function, just run it from main and insert any file you want.
    // stopWords("tiny.txt", index);


    return 0;
}

