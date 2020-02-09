/*****************************************************************************
 * COMP15: Data Structures, Fall 2019
 * Project 2: gerp - It's Like grep But Something Is Off
 * 
 * By:   Emily Hu
 * Date: 30 Nov 2019
 *
 * StringNode.cpp
 * Implementation of StringNode class
 ****************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <vector>

#include "StringNode.h"

using namespace std;

/* function: default constructor
 * does:     initializes StringNode class and sets member variable word to 
 *           empty string
 */
StringNode::StringNode()
{
	word = "";
}

/* function:   setWord
 * parameters: desired word
 * returns:    nothing
 * does:       sets member variable word to passed in parameter
 */
void StringNode::setWord(string input)
{
	word = input;
}

/* function:   getWord
 * parameters: none
 * returns:    string
 * does:       returns stored word
 *             
 */
string StringNode::getWord()
{
	return word;
}

/* function:   pushLineNum
 * parameters: number to be pushed
 * returns:    nothing
 * does:       pushes passed in num to back of linenums vector
 */
void StringNode::pushLineNum(int num)
{
	linenums.push_back(num);
}

/* function:   getLineNum
 * parameters: index
 * returns:    int
 * does:       returns the int in the linenums vector at the specified vector
 */
int StringNode::getLineNum(int i)
{
	return linenums.at(i);
}

/* function:   popLineNums
 * parameters: none
 * returns:    int
 * does:       returns the last int in the linenums vector
 */
int StringNode::popLineNum()
{
	return linenums.back();
}

/* function:   numAppearances
 * parameters: none
 * returns:    int
 * does:       returns the size of the linenums vector; equal to the number of 
 *             times that the word was found
 */
int StringNode::numAppearances()
{
	return linenums.size();
}

/* function:   pushFileNum
 * parameters: number to be pushed
 * returns:    nothing
 * does:       pushes passed in num to back of filenums vector
 */
void StringNode::pushFileNum(int num)
{
	filenums.push_back(num);
}

/* function:   getfilenum
 * parameters: index
 * returns:    int
 * does:       returns the int in the filenums vector at the specified vector
 */
int StringNode::getFileNum(int i)
{
	return filenums.at(i);
}

/* function:   popfilenum
 * parameters: none
 * returns:    int
 * does:       returns the last int in the filenums vector
 */
int StringNode::popFileNum()
{
	return filenums.back();
}


