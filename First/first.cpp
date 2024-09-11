#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <set>

#include "first.h"

/*
Do we need to consider the case

S -> aBa | bCb;
B -> a | b | D;
C -> Sef | fed;
C -> dog;

that non-terminal D doesn't exist on lhs
Then, maybe first(B) = {a, b, D}?
*/

using namespace std;
//******************************************************************************
node::node() {
    symbol = 0;
    next = nullptr;
}

//******************************************************************************
node::node(char sym) {
    symbol = sym;
    next = nullptr;
}

//******************************************************************************
Production::Production() {
    lhs = 0;
    rhs = nullptr;
    nextProduction = nullptr;
}

//******************************************************************************
Production::Production(char non_terminal) {
    lhs = non_terminal;
    rhs = nullptr;
    nextProduction = nullptr;
}

//******************************************************************************
Grammar::Grammar() {
    startProduction = nullptr;
    startSymbol = 0;
}

//******************************************************************************
// Grammar::~Grammar() {
//     Production *current = startProduction;
//     while (current != nullptr) {
//         Production *next = current->nextProduction;
//         node *currentNode = current->rhs;
//         while (currentNode != nullptr) {
//             node *nextNode = currentNode->next;
//             delete currentNode;
//             currentNode = nextNode;
//         }
//         delete current;
//         current = next;
//     }

//     startProduction = nullptr;
//     startSymbol = 0;
// }

//******************************************************************************
// Function to read grammar from file
int Grammar::readGrammar(string& filename) {
    int rc = -1;

    // Open the file
    ifstream file(filename);

    // Check if the file is open, if not print an error message
    if (!file.is_open()) {
        cout << "File not found" << endl;
        rc = -1;
    } else {
        string line;
        while (getline(file, line)) {
            // Skip empty lines
            if (line.empty()) {
                continue;
            }

            // The first symbol shall be the non-terminal in left hand side
            // Save the first symobl in the left hand side
            char lhs = line[0];

            // Save the first symbol as the start symbol 
            // if it is the first production
            if (startProduction == nullptr) {
                startSymbol = lhs;
            }

            // Create a new production
            Production *newProduction = new Production(lhs);

            // If there is no production yet
            // Save the new production as the first production
            if (startProduction == nullptr) {
                startProduction = newProduction;
            } else {
                // If other productions already exist
                // Add the new production to the end of the list
                Production *current = startProduction;

                // Move pointer to the last production
                while (current->nextProduction != nullptr) {
                    current = current->nextProduction;
                }
                current->nextProduction = newProduction;
            }

            // Create nodes for the right-hand side of the production
            node *prev = nullptr;

            // As first 4 symbols will be lhs and '->' so start from 4
            // Save each symbol in the rhs as a node
            for (size_t i = 4; i < line.length(); i++) {
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

        rc = 0;
        file.close();
    }

    return rc;
}

//******************************************************************************
// Function to print the grammar
void Grammar::printGrammar() {
    Production *current = startProduction;

    while (current != nullptr) {
        // Print lhs symbol and '->'
        cout << current->lhs << " -> ";

        // Print the rhs symbols
        node *currentNode = current->rhs;
        while (currentNode != nullptr) {
            cout << currentNode->symbol;
            currentNode = currentNode->next;
        }

        cout << endl;

        // Move to the next production
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
    Production *current = startProduction;

    while (current != nullptr) {
        // Since all lhs symbols are non-terminals
        // Start from the rhs symbols
        node *currentNode = current->rhs;

        // Insert all terminal symbols to the set of terminals
        while (currentNode != nullptr) {
            char symbol = currentNode->symbol;
            if (isTerminal(symbol)) {
                terminals.insert(symbol);
            }
            currentNode = currentNode->next;
        }

        // Move to the next production
        current = current->nextProduction;
    }

    // Print all terminal symbols
    cout << "Terminals: ";
    for (char terminal : terminals) {
        cout << terminal << " ";
    }
    cout << endl;
}

//******************************************************************************
// Function to print all non-terminals
void Grammar::printNonTerminals() {
    Production *current = startProduction;

    while (current != nullptr) {
        // Since all lhs symbols are non-terminals
        // Insert all lhs symbols to the set of non-terminals
        nonTerminals.insert(current->lhs);

        // For the case, where the rhs contains non-terminals
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

    // Print all non-terminal symbols
    cout << "Non-terminals: ";
    for (char nonTerminal : nonTerminals) {
        cout << nonTerminal << " ";
    }
    cout << endl;
}

//******************************************************************************
// Function to print the start symbol
void Grammar::printStartSymbol() {
    cout << "Start symbol: " << startSymbol << endl;
}

//******************************************************************************
bool Grammar::calculateFirst(char symbol) {
    bool rc = false;
    // If the symbol is a terminal or epsilon 
    // and exists in the set of terminals
    // add it to the first set
    // Print error message if given symbol is not found in the set of terminals
    if (isTerminal(symbol) || symbol == '&') {
        if(terminals.find(symbol) != terminals.end()){
            firstSets.insert(symbol);
            rc = true;
        } else{
            cout << "Terminal " << symbol 
            << " not found in the set of terminals" << endl;
            rc = false;
        }
    } else if(isNonTerminal(symbol)){
        // If the symbol is a non-terminal, but not in the set of non-terminals
        // Print error message
        if(nonTerminals.find(symbol) == nonTerminals.end()){
            cout << "Non-terminal " << symbol
            << " not found in the set of non-terminals" << endl;
            rc = false;
        } else{
            // Find production with the given non-terminal in the lhs
            Production *current = startProduction;
            
            while (current != nullptr){
                if (current->lhs == symbol){
                    // If the rhs of the production starts with a terminal
                    // add it to the first set
                    node *currentNode = current->rhs;
                    while(currentNode != nullptr){
                        if(isTerminal(currentNode->symbol) || currentNode->symbol == '&'){
                            firstSets.insert(currentNode->symbol);
                            rc = true;

                            // Move to the next alternative if '|' is found
                            while (currentNode->next != nullptr && currentNode->symbol != '|') {
                                currentNode = currentNode->next;
                            }
                        } else if(isNonTerminal(currentNode->symbol)){
                            // If the rhs starts with a non-terminal
                            // Do recursive call for current non-terminal
                            // Use a temporary node to keep the current node same
                            node *temp = currentNode;
                            calculateFirst(temp->symbol);

                            // Move to the next alternative if '|' is found
                            while (currentNode != nullptr && currentNode->symbol != '|') {
                                currentNode = currentNode->next;
                            }
                        }
                        currentNode = currentNode->next;
                    }
                }
                current = current->nextProduction;
            }
        }
    } else {
        // If the symbol is not a terminal or non-terminal
        // Print error message
        cout << "Symbol " << symbol << " is not a terminal or non-terminal" << endl;
        rc = false;
    }

    return rc;
}
//******************************************************************************
// Function to print the first sets
void Grammar::printFirstSets(char symbol) {
    
    // If the first set is calculated successfully
    if(calculateFirst(symbol)){
        cout << "First(" << symbol << ") = { ";
        for (char first : firstSets) {
            cout << first << " ";
        }
        cout << "}" << endl;
    } else {
        cout << "First(" << symbol << ") is not found" << endl;
    }

    // Clear the first set
    firstSets.clear();
}
