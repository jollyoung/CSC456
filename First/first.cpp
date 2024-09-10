#include <iostream>
#include <fstream>
#include <cstring>
#include <set>
#include "first.h"

using namespace std;

//******************************************************************************

// Constructor for Grammar class
Grammar::Grammar() : startProduction(nullptr), startSymbol(0) {}

//******************************************************************************

// Constructor for node class
node::node() : symbol(0), next(nullptr) {}

node::node(char sym) : symbol(sym), next(nullptr) {}

//******************************************************************************

// Constructor for Production class
Production::Production() : lhs(0), rhs(nullptr), nextProduction(nullptr) {}

Production::Production(char non_terminal) : lhs(non_terminal), rhs(nullptr), nextProduction(nullptr) {}

//******************************************************************************

// Function to read grammar from file
int Grammar::readGrammar(string& filename) {
    int rc = 0;

    // Open the file
    ifstream file(filename);

    // Check if the file is open, if not print an error message
    if (!file.is_open()) {
        cout << "File not found" << endl;
        return -1;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        // The first symbol shall be the non-terminal in left hand side
        char lhs = line[0];

        // Save the first symbol as the start symbol if this is the first production
        if (startProduction == nullptr) {
            startSymbol = lhs;
        }

        // Create a new production
        Production *newProduction = new Production(lhs);
        if (startProduction == nullptr) {
            startProduction = newProduction;
        } else {
            // Add the new production to the end of the list
            Production *current = startProduction;
            while (current->nextProduction != nullptr) {
                current = current->nextProduction;
            }
            current->nextProduction = newProduction;
        }

        // Create nodes for the right-hand side of the production
        node *prev = nullptr;
        for (int i = 4; i < line.length(); i++) {
            char sym = line[i];
            node *newNode = new node(sym);
            if (prev == nullptr) {
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

// Function to print the grammar
void Grammar::printGrammar() {
    Production *current = startProduction;
    while (current != nullptr) {
        cout << current->lhs << " -> ";
        node *currentNode = current->rhs;
        while (currentNode != nullptr) {
            cout << currentNode->symbol;
            currentNode = currentNode->next;
        }
        cout << endl;
        current = current->nextProduction;
    }
}

//******************************************************************************

// Helper function to check if a symbol is a non-terminal
bool Grammar::isNonTerminal(char symbol) {
    return symbol >= 'A' && symbol <= 'Z';
}

//******************************************************************************

// Helper function to check if a symbol is a terminal
bool Grammar::isTerminal(char symbol) {
    return symbol >= 'a' && symbol <= 'z';
}

//******************************************************************************

// Function to print all terminals
void Grammar::printTerminals() {
    set<char> terminals;
    Production *current = startProduction;
    while (current != nullptr) {
        node *currentNode = current->rhs;
        while (currentNode != nullptr) {
            char symbol = currentNode->symbol;
            if (isTerminal(symbol)) {
                terminals.insert(symbol);
            }
            currentNode = currentNode->next;
        }
        current = current->nextProduction;
    }

    cout << "Terminals: ";
    for (char terminal : terminals) {
        cout << terminal << " ";
    }
    cout << endl;
}

//******************************************************************************

// Function to print all non-terminals
void Grammar::printNonTerminals() {
    set<char> nonTerminals;
    Production *current = startProduction;
    while (current != nullptr) {
        nonTerminals.insert(current->lhs);
        node *currentNode = current->rhs;
        while (currentNode != nullptr) {
            char symbol = currentNode->symbol;
            if (isNonTerminal(symbol)) {
                nonTerminals.insert(symbol);
            }
            currentNode = currentNode->next;
        }
        current = current->nextProduction;
    }

    cout << "Non-terminals: ";
    for (char nonTerminal : nonTerminals) {
        cout << nonTerminal << " ";
    }
    cout << endl;
}

//******************************************************************************

// Main function
int main() {
    Grammar grammar;
    string filename = "grammar.txt";
    int rc = grammar.readGrammar(filename);
    if (rc == 0) {
        grammar.printGrammar();
        grammar.printTerminals();
        grammar.printNonTerminals();
    }
    return 0;
}
