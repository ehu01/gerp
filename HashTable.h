/*****************************************************************************
 * COMP15: Data Structures, Fall 2019
 * Project 2: gerp - It's Like grep But Something Is Off
 * 
 * By:   Emily Hu
 * Date: 30 Nov 2019
 *
 * HashTable.h
 * Header for HashTable class
 ****************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <functional>

#include "StringNode.h"

using namespace std;

const int INIT_SIZE = 1000;

class HashTable
{
public:

    /* parameterized constructor */
    HashTable(string directory);

    /* search function */
    bool search(string query, string directory, ofstream &out);


private:

    struct PermutationNode {
        string lowercase_word;
        vector<StringNode> permutations;
    };

    vector<vector<PermutationNode>> htable;

    vector<vector<string>> file_lines;

    hash<string> hash;


    size_t num_elements = 0;
    size_t table_size = INIT_SIZE;

    /* build hash table - called by constructor */
    void build(string directory);

    /* traverse the file tree */
    int traverse(DirNode *root, string fullpath, string path, size_t num_files);

    /* look at each word in file */
    void readFile(string file, string path, size_t filenum);

    /* insert word into hash table */
    void insert(string word, string line, int num_lines, int filenum);

    /* helper for readFile to look for duplicates */
    bool duplicates(int index, size_t num_lines, int filenum, 
                                                    string line, string word);
    /* check all permutations */
    bool checkPerms(int index, int depth, size_t num_lines, int perms,
                                       int filenum, string line, string word);
    /* strips non alpha numeric chars */
    string stripNonAlphaNum(string input);

    /* helper to search function */
    void searchTable(string original, string lower, string org_dir, 
                                             bool insensitive, ofstream &out);

    /* hash table expansion */
    void expandTable();

    /* calculate load factor */
    float loadFactor();

    /* converts string to all lowercase */
    string lowercase(string upper);

};
