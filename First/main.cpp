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

int main() {
    Grammar grammar;
    string filename = "grammar.txt";
    int rc = grammar.readGrammar(filename);
    if (rc == 0) {
        grammar.printAll();
        grammar.printFirstSets('d');
        grammar.printFirstSets('B');
        grammar.printFirstSets('S');
        grammar.printFirstSets("Ba");
    }

    return 0;
}