//*search.h*/

// This file takes in a simple file from the main fuction in the cpp file
// and makes a inverted index anyone can search through using keys and values.
//
// Anant Raj Saroya
// Ned id: 671783409
// U. of Illinois, Chicago

#pragma once

#include <iostream>
#include <set>
#include <map>
#include <fstream>
#include <cctype>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;


//This function takes in individual words and removes all punctuation from the front and back of that word.
//The function does not remove any punction in between the word. 
//If a word is empty then it will store that word as a empty string at the end.
string cleanToken(string& s) {
    int count = 0;
    
    if (s.empty())        
    {
        s = "";
        return s;
    }
    for (int i = 0; i < s.size(); i++)
    {
        if (isalpha(s.at(i)))  
        {
            count++;
        }
    }
    if (count == 0)
    {
        s = "";
        return s;
    }
    for (int i = 0; i < s.size(); i++)
    {
        if (isalpha(s[i]))
        {
            count++;
            if (count < 1)
            {
                s[i] = ' ';
            }
        }
    }
    if (s.size() != 0)
    {
        while(ispunct(s.front()))
        {
            s.erase(s.begin());
        }
    }
    if (s.size() != 0)
    {
        while (ispunct(s.back()))
        {
            s.pop_back();
        }
    }
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (isalpha(s[i]))
        {
            s[i] = tolower(s[i]);
        }
    }
    return s;
}

//This function takes in the body test from a file and seperates it by space using stringstream.
//Then those words are used to make key's using using cleanToken().
//At the very end it takes those clean tokens and stores them in a set to be used later on.
set<string> gatherTokens(string text) {
    set<string> tokens;
    string word;

    stringstream s(text);
    string variable;
    while (s >> word)
    {
        variable = cleanToken(word);
        if (!variable.empty())
        {
            tokens.insert(variable);
        }
    }
    /*for (auto it = tokens.begin(); it !=   //checks tokens are being stored properly
        tokens.end(); ++it)
    {
        cout << " " << *it;
        //cout << tokens.size();
    }*/

    return tokens;
}

//This function takes in a file and a map that will be filled with keys and values. 
//The first line is read in as values (URL's) and the second line is being read in as Tokens(Body).
//Every two lines are a pair so it is going through the file pairing every two lines together.
int buildIndex(string filename, map<string, set<string>>& index) {
    string value;
    set<string> keytokens;
    string key;
    int count = 0;

    ifstream infile(filename);
    if (infile.fail())
    {
        cout << "Invalid file" << endl;
        return 0;
    }
    while (infile.good())
    {
        getline(infile, value, '\n');
        //cout << "value: " << value << endl;
        getline(infile, key, '\n');
        //cout << "key: " << key;
        count++;
        if (!infile.fail())
        {
            keytokens = gatherTokens(key);
            //cout << keytokens.size() << endl;
            for (auto E : keytokens)
            {
               index[E].insert(value);
            }
        }
    }
    //tests below to see if keytoken is being filled properly and map is taking in correct keys and values
    /*for (auto it = keytokens.begin(); it !=
            keytokens.end(); ++it)
    {
        cout << " " << *it;
        //cout << tokens.size();
    }*/
    
    /*for (auto E : index)
    {
        cout << E.first << " ";
        //count++;

        for (auto A : E.second)
        {
           //count++;
           cout << A << " ";
        }
        cout << endl;
    }*/
    
    cout << "Indexed " << count - 1 << " pages containing " << index.size() << " unique terms " << endl << endl;

    return count - 1;  //return pages indexed
}

//This function takes in a sentence from the user, splits it into individual words and uses those terms as keys to look through the map.
//The user can add a + or - in the begining of the word to change the way that keytoken will be searched in the map.
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
    set<string> result;
    set<string> settemp;
    set<string> settemp2;
    set<string> settemp3;
    set<string> setinter;
    set<string> empty;       //empty set to merge with other sets without any overlap
    set<string> dif;
    
    string temp;
    string word;

    stringstream x(sentence);
    //cout << sentence;
    x >> temp;
    for (auto it : index)
    {
        if (it.first == temp)
        {

            for (auto A : it.second)
            {
                result.insert(A);
            }
        }
    }

    while (x >> temp)
    {   
        if (temp.empty())
        {
            cout << "empty";
            return result;
        }
        //cout << temp << endl;
        if (temp.at(0) == '+')
        {
            word = cleanToken(temp);
            //cout << "word: " << word << endl;         
            for (auto it : index)
            {
                if (it.first == temp)
                {

                    for (auto A : it.second)
                    {
                        settemp.insert(A);
                    }
                }
            }
            if(dif.empty())
            {
                //algorithm takes in two sets and outputs the intersections into another set.
                set_intersection(result.begin(), result.end(), settemp.begin(), settemp.end(), inserter(setinter, setinter.begin()));
                result.clear();
                //almost the same as inserting into a set but this lets you merge two sets into one and put them into another set
                merge(empty.begin(), empty.end(), setinter.begin(), setinter.end(), inserter(result, result.begin()));
            }
            else
            {
                //algorithm takes in two sets and outputs the intersections into another set.
                set_intersection(dif.begin(), dif.end(), settemp.begin(), settemp.end(), inserter(setinter, setinter.begin()));  
                result.clear();
                //almost the same as inserting into a set but this lets you merge two sets into one and put them into another set
                merge(empty.begin(), empty.end(), setinter.begin(), setinter.end(), inserter(result, result.begin()));
            }

            /*cout << " +Result set : " << endl;            //checking set result is being used properly
            for (auto it = setinter.begin(); it !=
                setinter.end(); ++it)
            {
                //count++;
                cout << " " << *it;
                cout << endl;
            }
            //cout << "Found " << result.size() << " matching pages" << endl;
            cout << endl;*/
        }
        else if (temp.at(0) == '-')
        {
            word = cleanToken(temp);
            //cout << "word: " << word << endl;
            for (auto it : index)
            {
                if (it.first == temp)
                {
                    for (auto A : it.second)
                    {
                        settemp3.insert(A);
                    }
                }
            }
            if (setinter.empty())
            {
                set_difference(result.begin(), result.end(), settemp3.begin(), settemp3.end(), inserter(dif, dif.begin()));
                result.clear();
                merge(empty.begin(), empty.end(), dif.begin(), dif.end(), inserter(result, result.begin()));
            }
            else
            {
                set_difference(setinter.begin(), setinter.end(), settemp3.begin(), settemp3.end(), inserter(dif, dif.begin()));
                result.clear();
                merge(empty.begin(), empty.end(), dif.begin(), dif.end(), inserter(result, result.begin()));
            }
            
            /*cout << " - Result set : " << endl;
            for (auto it = result.begin(); it !=
                result.end(); ++it)
            {
                //count++;
                cout << " " << *it;
                cout << endl;
            }
            //cout << "Found " << result.size() << " matching pages" << endl;
            cout << endl;*/
        }
        else
        {
            word = cleanToken(temp);
            for (auto it: index)
            {
                if (it.first == word)
                {
                        for (auto A : it.second)
                        {
                            result.insert(A);
                        }
                    
                }
            }
        }
    }

    cout << "Found " << result.size() << " matching pages" << endl;
    for (auto it = result.begin(); it !=
        result.end(); ++it)
    {
        cout << "" << *it;
        cout << endl;
    }

    return result;
}

//This function takes in a filename and uses that file to make a Inverted Index using buildIndex().
//After building the Inverted Index the function then takes in a command from the user and send that to findQueryMatches() alongside the map.
//The function outputs the webpages that match the keys in query while query does NOT equal '\n'
void searchEngine(string filename) {
    map<string, set<string>> mymap;
    string query;

    cout << "Stand by while building index..." << endl;
    buildIndex(filename, mymap);
    cout << "Enter query sentence (press enter to quit): ";
    getline(cin, query);
    while (query.length() != 0)
    {
        findQueryMatches(mymap, query);
        cout << endl;
        cout << "Enter query sentence (press enter to quit): ";
        getline(cin,query);
    }
    if (query.length() == 0)
    {
        cout << "Thank you for searching!" << endl;
    }
}
