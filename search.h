/*

Program 2: Search Engine
Course: CS 251
Author: Daniel Luangnikone dluan3@uic.edu
System: Windows 11, using VSC

This program runs a search engine from user input and finds matches

*/

#pragma once

#include <iostream>
#include <set>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;


/* This function takes a string and gets rids off all punction in front of the string and the end of the string (I.E leading and trailing punction)
the for loop loops thru the string and checks if any of the letters within the string are alphabets, if thats true then it increments a counter since we want to check if its true for atleast a letter
if the counter is zero, it returns an empty string
however if the counter is bigger than zero, it gets rids of leading and trailing punction
then returns the lowercase word */
string cleanToken(string s) 
{    
    int counter = 0; // keep a counter, used for seeing if atleast one char is present
    for(auto &letters: s) 
    {
        if(isalpha(letters))
        {
            counter++; // increment for how many letters there are
        }
    }
    if(counter == 0)
    {
        return ""; // return a empty string if theres no letters to work with
    }
    if(counter > 0) // if theres at least a letter present
    {
        for(auto &letter : s) 
        {
            if(ispunct(s.front())) // if the front has punct
            {
                s.erase(s.begin()); // erase until you reach the first letter
            }
            if(ispunct(s.back())) // if the back has punct
            {
                s.pop_back(); // erase until you reach the last letter
            }
            letter = tolower(letter); // final step is to lowercase every letter
        }
        return s;
    }
}

/* this function inserts tokens from a given text or sentence and inserts it into a set */
set<string> gatherTokens(string text) 
{
    set<string> tokens;
    stringstream ss(text);
    string value;
    int counter = 0;
    
    while(getline(ss, value, ' ')) // delimit by spaces
    {
        tokens.insert(cleanToken(value)); // insert a clean version of the token into the set
        if(cleanToken(value) == "") // if theres an empty string
        {
            tokens.erase(cleanToken(value)); // erase empty string
        }
    }
    return tokens;
}

/* buildIndex again takes a file and parses it by lines so that we can seperate the given url and the body text that it contains. 
After that, we accociate a given token from the parsed verison of the line and accociate it with it's given url*/
int buildIndex(string filename, map<string, set<string>>& index) 
{
    string value, line;
    int counter = 0;
    vector<string> websites;
    string website;
    
    ifstream infile;
    infile.open(filename);

    if(infile) // if valid file to be opened
    {
        while(!infile.eof())
        {
            if(getline(infile, line))
            {
                websites.push_back(line); // push back the whole line within the file to a vector
            }
        }
    }
    else // if invalid, then exit
    {
        return 0;
    }
    for(int i = 0; i < websites.size(); i++)
    {
        if(i % 2 == 0) // lines belong on the even index when looping thru a file, so if its even 
        {
            website = websites.at(i); // set website to the current website of the file
            counter++; // counter keeps track of the size
        }
        else
        {
          for(auto &tokens: gatherTokens(websites.at(i))) // each token is accociated with a set of websites
            {
                index[cleanToken(tokens)].insert(website); // insert the token accociated with the website
            }   
        }
    } 
    cout << "Indexed " << counter << " pages containing " << index.size() << " unique terms" << endl; // cout statement for the search engine
    return counter;
}

/* this function takes words from a given sentence and does set operations given if the word within the setence is prefaced with a plus or minus or a space*/
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) 
{
    set<string> result, first, temp;
    vector<string> words;
    stringstream ss(sentence);
    string value;

    while(getline(ss, value, ' ')) // delimit by spaces again
    {
        words.push_back(value); // push back each word into a vector
    }

    for(int i = 0; i < words.size(); i++) 
    {
        first = index[cleanToken(words.at(0))]; // get the first word within the vector
    }

    if(words.size() == 1) // if the size is only one that means theres only one word within the vector
    {
        result = first; // if only one word, just put into result 
    }
    else if(words.size() > 1) // if there's more than one word
    {
       for(int i = 0; i < words.size(); i++)
        {
            if(index.count(cleanToken(words.at(i)))) // if there's a word that is in index
            {
                if(words.at(i).front() == '+') // if the front of the word is prefaced with a +
                {
                    result.clear();
                    set_intersection(first.begin(), first.end(), index[cleanToken(words.at(i))].begin(), index[cleanToken(words.at(i))].end(), inserter(result, result.end())); // set operation
                }
                else if(words.at(i).front() == '-') // if the front of the word is prefaced with a -
                {
                    set_difference(result.begin(), result.end(), index[cleanToken(words.at(i))].begin(), index[cleanToken(words.at(i))].end(), inserter(temp, temp.end())); // in this case, you have to minus from the result set since there's already content within the result set and insert into a temporary set to hold the content
                    result.clear(); // clear because we dont want redundancy
                    for(auto &content : temp)
                    {
                        result.insert(content); // insert everyhing from temp into result
                    }
                }
                else 
                {
                    set_union(first.begin(), first.end(), index[cleanToken(words.at(i))].begin(), index[cleanToken(words.at(i))].end(), inserter(result, result.end())); // set operation
                }
            }
        } 
    }
    
    if(result.size() > 0) 
    {
        cout << endl;
        cout << "Found " << result.size() << " matching pages" << endl; // cout for search engine
    }

    for(auto &words : result)
    {
        cout << words << endl; // cout content from result
    }

    return result;
}

/* This function takes all the previous functions and just combines them to build a search engine */
void searchEngine(string filename) 
{
    map<string, set<string>> index;
    string userinput = "LOL"; // initalize this to have a value because by default if you dont, then it will just be an empty string. 

    cout << "Stand by while building index..." << endl;
    buildIndex(filename, index); // build index

    while(userinput != "")
    {
        cout << endl;
        cout << "Enter query sentence (press enter to quit): "; // loop to prompt for userinput
        getline(cin, userinput);
        findQueryMatches(index, userinput);
    } 
    if(userinput == "") // if the user quits
    {
        cout << "Thank you for searching!" << endl; // see ya
    }

}

// creative function, takes a list of stop words and loops thru the index. See if index contains a stopword from the list, then removes it from index.
// to use this function, just run it from main and insert any file you want.
void stopWords(string filename, map<string, set<string>>& index)
{
    vector<string> stopWords = {"the", "and", "to", "a", "an", "for", "but", "or", "so", "at", "i'm", "is", "it", "its", "on", "off", "no", "not"};
    map<string, set<string>>::iterator it;
    
    buildIndex(filename, index);

    cout << "before removal of stopwords. "; // to see intial size
    cout << index.size() << endl;

    for(int i = 0; i < stopWords.size(); i++) // loop thru the vector of stop words
    {
        if(index.count(cleanToken(stopWords.at(i)))) // if index contains a stop word
        {
            index.erase(cleanToken(stopWords.at(i))); // remove stop word
        }
    }

    cout << "after removal of stopwords. "; // see the size after the removal
    cout << index.size() << endl;
}