/*
* Rolling Pandas
* CSC453 Fall 2024
* Assignment: First part 1
*/

#include "first.h"

int main() {
    Grammar grammar;
    string filename = "grammar.txt";
    int rc = grammar.readGrammar(filename);
    if (rc == 0) {
        grammar.printAll();
    }

    filename = "grammar2.txt";
    rc = grammar.readGrammar(filename);
    if (rc == 0) {
        grammar.printAll();
        
    }
    return 0;
}