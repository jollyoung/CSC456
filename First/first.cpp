#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "first.h"
#include "grammar.txt"

#define MAX 10

using namespace std;

// Structure to store the grammar
Grammar::Grammar() {
    int numProductions = 0;
};

// Function to calculate FIRST
void Grammar::calculateFirst(char c) {
    int i, j;
    // If the symbol is lowercase(non-terminal), then add it to the FIRST set
    if(!(isupper(c))) {
        first = c;
    }
}

void Grammar::readGrammar(const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        strcpy(production[numProductions++], line.c_str());
    }
}

int main() {
    Grammar g;
    g.readGrammar("grammar.txt");

    char c;


    return 0;
}