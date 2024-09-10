#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "first.h"


using namespace std;

//******************************************************************************
int Grammar::readGrammar(string& filename) {
    int rc = 0;

    // Open the file
    ifstream file(filename);

    // Check if the file is open, if not print an error message
    if (!file.is_open()) {
        cout << "File not found" << endl;
        rc = -1;
    }

    string line;
    while(getline(file,line)){
        if (line.empty()) {
            continue;
        }

        // The first symbol shall be the non-terminal in left hand side
        char lhs = line[0];

        // Save the first symbol as the start symbol
        if(startProduction == nullptr) {
            startSymbol = lhs;
        }

        // Create a new production
        Production *newProduction = new Production(lhs);
        if(startProduction == nullptr) {
            startProduction = newProduction;
        } else {
            // If there are already productions, add the new production to the end
            Production *current = startProduction;
            while(current->nextProduction != nullptr) {
                current = current->nextProduction;
            }
            current->nextProduction = newProduction;
        }

        // Create a new node for the first symbol in the right hand side
        node* prev = nullptr;
        for(int i = 2; i < line.length(); i++) {
            char sym = line[i];
            node* newNode = new node(sym);
            if(prev == nullptr) {
                newProduction->rhs = newNode;
            } else {
                prev->next = newNode;
            }
            prev = newNode;
        }

    }

    file.close();

    return rc;
}

//******************************************************************************
void Grammar::printGrammar() {
    Production *current = startProduction;
    while(current != nullptr) {
        cout << current->lhs << " -> ";
        node *currentNode = current->rhs;
        while(currentNode != nullptr) {
            cout << currentNode->symbol;
            currentNode = currentNode->next;
        }
        cout << endl;
        current = current->nextProduction;
    }
}

//******************************************************************************
int main() {
    Grammar grammar;
    string filename = "grammar.txt";
    int rc = grammar.readGrammar(filename);
    if(rc == 0) {
        grammar.printGrammar();
    }

    return 0;
}