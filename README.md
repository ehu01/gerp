/*****************************************************************************
 * COMP15: Data Structures, Fall 2019
 * Project 2: gerp - It's Like grep But Something Is Off
 *
 * By: Emily Hu
 * Date: 7 December 2019
 * 
 * README
 *****************************************************************************/


COMPILE/RUN:

     - Compile and link using
            make gerp
     - Run executable with
            ./gerp inputDirectory outputFile
     - Query with
            [query]                     to search case sensitively
            @i OR @insensitive [query]  to search case insensitively
            @q OR @quit                 to quit program


PURPOSE:

The goal of this project was to build a C++ implementation of 'grep' given a 
directory and a file tree. We were given freedom to construct our data structure
as we saw fit which we would then search for a specified string. 


FILES:

main.cpp
    Program driver. Builds the hash table, takes user queries, and searches.

HashTable.h
    Interface of HashTable class. Includes definition of the PermutationNode
    struct.

HashTable.cpp
    Implementation of HashTable class. Contains functionality for traversing
    file tree, parsing files, inserting strings into hash table, and searching
    for strings in hash table.

StringNode.h
    Interface of StringNode class.

StringNode.cpp
    Implementation of the StringNode class. Contatins functionality for getting
    and setting the word member variable and various parts of the filenums and 
    linenums vectors.

FSTree.h
    PROVIDED AS STARTER CODE: Interface of FSTree class which builds a file
    tree when given a directory. 

DirNode.h
    PROVIDED AS STARTER CODE: Interface of DirNode class, used when traversing
    file tree.


DATA STRUCTURES:

HashTable
    * 2-D vector table: vector of vectors
    * collisions are handled by pushing to the back of the inner vector at the
      hashed index
    * searching time is fast this way -- ideally constant time, if chains are 
      short
    * load factor used is 0.8
    * each string is converted to lowercase and hashed to get key
    * value is a struct which holds the lowercase version of the string as well
      as a vector containing every unique case permutation of the string which
      was encountered in the form of a StringNode object

StringNode
    * contains two parallel vectors of integers
        * filenums holds each # file the string appears in
        * linenums holds each # line the string appears in
    * a filenum and a linenum at the same index act as a coordinate pair which
      is used to find the the exact file and line they refer to in the 
      file_lines data structure

file_lines
    * 2-D vector table: vector of vectors
    * stores file paths and file lines
    * each inner vector holds, at the 0th index, the full file name
        * at each following index, it holds that number line of that file
    * for example, if StringNode x appears in file number 4 and line number 8,
      looking at file_lines[4][8] would get the full line and 
      looking at file_lines[4][0] would get the full file path
        * this way, each line and path is not stored more than once.


TESTING:

Unit testing on functions was done in a testing_main file. 
        
Testing the entire program most heavily involved running the same commands with
the reference implementation, sorting both output files, and running the diff
command. Edge cases tested include empty strings, multiple searches in one line,
case insensitive number searches, only non-alphanumeric strings, case sensitive
searches with different permutations of the same word, case insensitive searches
of different permutations of the same word.

Directories tested ranged from a few files with a few lines each to a directory
which contained about 8 GB of data.


NOTES:

Multiple queries can be given in one line and the program will conduct each
search individually. 

This project required careful consideration of space and time utilization. This
program had to process and store up to 8 GB of data without overloading the 
homework server and without taking over 10 minutes to traverse the entire file
tree. My program takes approximately 50 seconds to be ready to query when given 
largest test file (8 GB) and utilizes a data storage system which turned out to 
be more efficient than the reference implementation provided by the course 
staff.

FSTree.h and DirNode.h were provided as starter code as the goal of this project
was not to practice creating a file tree but to build our own data structure and
practice managing storage efficiently.

