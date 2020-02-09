/*****************************************************************************
 * COMP15: Data Structures, Fall 2019
 * Project 2: gerp - It's Like grep But Something Is Off
 * 
 * By:   Emily Hu
 * Date: 30 Nov 2019
 *
 * HashTable.cpp
 * Implementation of HashTable class
 ****************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>

#include "HashTable.h"

using namespace std;

const float LOAD_FACTOR = 0.8;


/* function:   parameterized constructor
 * parameters: directory to index
 * does:       calls build() to populate hash table\
 */
HashTable::HashTable(string directory)
{
    htable.resize(table_size);

    file_lines.resize(table_size);

    build(directory);
}


/* function:   build
 * parameters: directory to index
 * returns:    nothing
 * does:       creates FSTree instance, calls function to traverse the tree
 */
void HashTable::build(string directory)
{
    FSTree filetree(directory);
    
    traverse(filetree.getRoot(), directory, "", 0);
}


/* function:   traverse
 * parameters: dirnode, file name, directories this function has traversed
 *             through, number of files currently found
 * returns:    current total number of files
 * does:       traverses recursively through the FSTree, calling readFile on
 *             each file found
 */
int HashTable::traverse(DirNode *dirnode, string fullpath, string path,
                                                               size_t num_files)
{
    ofstream outfile;
    outfile.open("output.txt");

    int number_subDirs = dirnode->numSubDirs();

    for (int i = 0; i < number_subDirs; i++) {
        string fulldir = fullpath + "/" + dirnode->getSubDir(i)->getName();
        string partdir = path + "/" + dirnode->getSubDir(i)->getName();

        num_files 
            = traverse(dirnode->getSubDir(i), fulldir, partdir, num_files);
    }

    int number_files = dirnode->numFiles();

    for (int i = 0; i < number_files; i++) {
        string fulldir = fullpath + "/" + dirnode->getFile(i);
        string partdir = path + "/" + dirnode->getFile(i);

        num_files++;

        readFile(fulldir, partdir, num_files);
    }
    return num_files;
} 


/* function:   readFile
 * parameters: file name, partial path, # file that it is
 * returns:    nothing
 * does:       parses through file to look at each word individually and insert
 *             it into the hash table
 */
void HashTable::readFile(string file, string path, size_t filenum)
{
    ifstream infile;
    infile.open(file);
    if (!infile.is_open()) {
        cout << "error opening " << file << endl;
        return;
    }

    string current   = "";
    string line      = "";
    size_t num_lines =  0;

    if (filenum >= file_lines.size()) {
        file_lines.resize(file_lines.size() * 2 + 2);
    }
    file_lines[filenum].push_back(path);

    while (getline(infile, line)) {
        num_lines++;
        file_lines[filenum].push_back(line);
        stringstream ss(line);

        while (ss >> current) {
            if (loadFactor() >= LOAD_FACTOR) {
                expandTable();
            }
            insert(current, line, num_lines, filenum);
        }
    }
    infile.close();
}


/* function:   insert
 * parameters: word to insert, line it was found in, # file and # line it was
 *             found in
 * returns:    nothing
 * does:       hashes the word and inserts it depending on whether the exact
 *             word was found before, a permutation of it was found before,
 *             or if it is new
 */
void HashTable::insert(string word, string line, int num_lines, int filenum)
{
    word = stripNonAlphaNum(word);
    string lower_word = lowercase(word);
    size_t index = hash(lower_word) % table_size;
    bool found;

    // searching for duplicates
    if (!htable.at(index).empty()) {
        found = duplicates(index, num_lines, filenum, line, word);
    } else {
        found = false;
    }

    if (not found) {

        PermutationNode perm;
        perm.lowercase_word = lower_word;

        StringNode new_word;
        new_word.setWord(word);
        new_word.pushLineNum(num_lines);
        new_word.pushFileNum(filenum);
        
        perm.permutations.push_back(new_word);
        
        htable[index].push_back(perm);

        num_elements++;
    }
}


/* function:   duplicates()
 * parameters: hashed index, # line and # file word was found in, full line,
 *             and the word
 * returns:    boolean
 * does:       determines whether the current word exists in the hash table
 *             already and if it does, inserts it in the right place
 */
bool HashTable::duplicates(int index, size_t num_lines, int filenum,
                                                       string line, string word)
{
    size_t depth = htable[index].size();

    string lower = lowercase(word);

    for (size_t i = 0; i < depth; i++) {

        // check if the lowercase version of current word has been found before
        if (htable[index][i].lowercase_word == lower) {

            size_t num_perms = htable[index][i].permutations.size();

            if (checkPerms(index, i, num_lines, num_perms, filenum, line, word))
                return true;

            // if current permutation has not been found but lowercase has
            StringNode new_word;
            new_word.setWord(word);
            new_word.pushLineNum(num_lines);
            new_word.pushFileNum(filenum);
            
            htable[index][i].permutations.push_back(new_word);

            return true; 
        }
    }
    return false;
}


/* function:   checkPerms
 * parameters: hashed index, # spot in the chain, # line, number of
 *             permutations, # file, full line, the word
 * returns:    boolean
 * does:       determines whether current permutation of word exists already 
 *             within hash table and inserts accordingly
 */
bool HashTable::checkPerms(int index, int depth, size_t num_lines, int perms,
                                          int filenum, string line, string word) 
{
    for (int j = 0; j < perms; j++) {

        // check if current permutation has been found before
        if (htable[index][depth].permutations[j].getWord() == word) {

            // check if current permutation was found on same line
            int file_number = htable[index][depth].permutations[j].popFileNum();
            int line_number = htable[index][depth].permutations[j].popLineNum();

            if (line == file_lines[file_number][line_number] &&
                num_lines == (size_t)line_number) {
                return true;
            }

            // if current perm has been found but not on same line...
            htable[index][depth].permutations[j].pushLineNum(num_lines);
            htable[index][depth].permutations[j].pushFileNum(filenum);

            return true;       
        }
    }
    return false;
}


/* function:   search
 * parameters: user query, directory, file stream
 * returns:    boolean
 * does:       determines what to do based on user's input, calls helper
 *             searchTable to find and print all instances of the word
 */
bool HashTable::search(string query, string directory, ofstream &out)
{
    string file;
    // conduct case insensitive search
    if (query == "@i" or query == "@insensitive") {
        cin >> query;
        query = stripNonAlphaNum(query);
        searchTable(query, lowercase(query), directory, true, out);
    // quit program
    } else if (query == "@q" or query == "@quit") {
        cout << endl << "Goodbye! Thank you and have a nice day." << endl;
        return true;
    // change output file
    } else if (query == "@f") {
        cin >> file;
        out.close();
        out.open(file);
    // conduct case sensitive search
    } else {
        query = stripNonAlphaNum(query);
        searchTable(query, lowercase(query), directory, false, out);
    }

    return false;
}


/* function:   searchTable
 * parameters: word, lowercase version of word, directory, case sensitivity
 *             (bool), file stream
 * returns:    nothing
 * does:       searches hash table for instances of word, prints them out
 * notes:      this function slightly longer due to extra line spacing for ease
 *             of reading
 */
void HashTable::searchTable(string original, string lower, string org_dir, 
                                               bool insensitive, ofstream &out)
{
    size_t index = hash(lower) % table_size;
    for (size_t i = 0; i < htable[index].size(); i++) {
        
        if (htable[index][i].lowercase_word == lower) {
            
            for (size_t j = 0; j < htable[index][i].permutations.size(); j++) {
                
                if (!insensitive) {
                    if (htable[index][i].permutations[j].getWord() != original){
                        continue;
                    }
                }
                int times = htable[index][i].permutations[j].numAppearances();
                for (int k = 0; k < times; k++) {
                    
                    int file_number  
                               = htable[index][i].permutations[j].getFileNum(k);
                    int line_number  
                               = htable[index][i].permutations[j].getLineNum(k);                    
                    
                    string full_path = org_dir + file_lines[file_number][0];
                    string full_line = file_lines[file_number][line_number];

                    out << full_path   << ":"
                        << line_number << ": "
                        << full_line   << endl;
                }
                if (!insensitive)
                    return;
            } 
            if (insensitive) 
                return;
        }
    }
    if (!insensitive)
        out << original << " Not Found. Try with @insensitive or @i." << endl;
    else
        out << lower << " Not Found." << endl;
}


/* function:    stripNonAlphaNum
 * parameters:  input string
 * returns:     same string but with non alphanumeric characters stripped from
 *              beginning and end
 * does:        removes non alphanumeric characters from beginning and end of 
 *              string
 */
string HashTable::stripNonAlphaNum(string input)
{
    int length = input.length();

    int i = 0;
    while (length > 0 and (input[i] < '0' or (input[i] > '9' and input[i] < 'A') 
       or (input[i] > 'Z' and input[i] < 'a') or input[i] > 'z')) {
        i++;
        length--;
    }

    int j = input.length() - 1;
    while (length > 0 and (input[j] < '0' or (input[j] > '9' and input[j] < 'A') 
       or (input[j] > 'Z' and input[j] < 'a') or input[j] > 'z')) {
        j--;
        length--;
    }

    string output = "";

    for (int k = i; k <= j; k++) {
        output += input[k];
    }

    return output;
}


/* function:   expandTable
 * parameters: none
 * returns:    nothing
 * does:       re-hashes everything within original hash table into new, bigger
 *             hash table, copies back into original hash table
 */
void HashTable::expandTable()
{   
    vector<vector<PermutationNode>> temp_htable;

    table_size = table_size * 2 + 2;

    temp_htable.resize(table_size);

    for (size_t i = 0; i < htable.size(); i++) {
        for (size_t j = 0; j < htable[i].size(); j++) {
            int index = hash(htable[i][j].lowercase_word) % table_size;
            temp_htable[index].push_back(htable[i][j]);
        }
    }

    htable.clear();

    htable = temp_htable;

    temp_htable.clear();
}


/* function:   loadFactor
 * parameters: none
 * returns:    current load factor
 * does:       calculates load factor using private member variables, returns it
 */
float HashTable::loadFactor()
{
    return num_elements / table_size;
}


/* function:   lowercase
 * parameters: string to be converted
 * returns:    lowercase version of string
 * does:       converts specified string to all lowercase
 */
string HashTable::lowercase(string upper)
{
    string lower = "";

    for (size_t i = 0; i < upper.length(); i++) {
        lower += tolower(upper[i]);
    }

    return lower;
}

