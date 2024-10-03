/*
* Rolling Pandas
* CSC453 Fall 2024
* Assignment: First part 1
*/
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "first.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input file>" << endl;
        return 1;
    }

    Grammar grammar;
    string filename = argv[1];  

    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: could not open input file " << filename << endl;
        return 1;
    }

    cin.rdbuf(inputFile.rdbuf());

    int rc = grammar.readGrammar(filename);
    if (rc == 0) {
        grammar.printAll();
        grammar.printFirstAll();
    }

    inputFile.close(); 

    return 0;
}
