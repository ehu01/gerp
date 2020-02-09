/*****************************************************************************
 * COMP15: Data Structures, Fall 2019
 * Project 2: gerp - It's Like grep But Something Is Off
 *
 * By:   Emily Hu
 * Date: 30 Nov 2019
 *
 * StringNode.h
 * Header for StringNode class
 ****************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <vector>

#include "FSTree.h"

using namespace std;

class StringNode
{
public:

    /* default constructor */
    StringNode();

    /* getters & setters */
    void   setWord(string input);
    string getWord();
    void   pushLineNum(int num);
    int    getLineNum(int i);
    int    popLineNum();
    void   pushFileNum(int num);
    int    getFileNum(int i);
    int    popFileNum();

    /* number of times a word appears */
    int    numAppearances();

private:

    string word;

    vector<int>    linenums;  
    vector<int>    filenums;   
};
