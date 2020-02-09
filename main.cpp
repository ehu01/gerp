/*****************************************************************************
 * COMP15: Data Structures, Fall 2019
 * Project 2: gerp - It's Like grep But Something Is Off
 * 
 * By:   Emily Hu
 * Date: 30 Nov 2019
 *
 * main.cpp
 * Program Driver
 ****************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>

#include "HashTable.h"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 3) {
		cout << "Usage: ./gerp inputDirectory outputFile\n";
		return EXIT_FAILURE;
	}

	string directory  = argv[1];
	string outputFile = argv[2];

	HashTable htable(directory);

	string query;

	ofstream outfile;
	outfile.open(outputFile);

	bool quit = false;

	while (!quit) {
		cout << "Query? ";
		cin >> query;
		quit = htable.search(query, directory, outfile);
		if (quit) 
			return 0;
		cout << endl;
	} 	
}
