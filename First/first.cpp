/*
* Rolling Pandas
* CSC453 Fall 2024
* Assignment: First part 1
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <set>

#include "first.h"

using namespace std;
/*
TO DO LIST
1. Implement code to check for characters like -> or ;
2. Implement deconstructors
3. When the pointer meats ;, use it as a sign to go next line. No other
symbols
*/
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
node::~node() {
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
Production::~Production() {
    node *currentNode = rhs;
    while (currentNode != nullptr) {
        node *nextNode = currentNode->next;
        delete currentNode;
        currentNode = nextNode;
    }
    rhs = nullptr;
    nextProduction = nullptr;
}

//******************************************************************************
Grammar::Grammar() {
    startProduction = nullptr;
    startSymbol = 0;
}

//******************************************************************************
Grammar::~Grammar() {
    Production *currentProduction = startProduction;
    while (currentProduction != nullptr) {
        Production *nextProduction = currentProduction->nextProduction;
        delete currentProduction;
        currentProduction = nextProduction;
    }
    startProduction = nullptr;
}

//******************************************************************************
// Function to read grammar from file
int Grammar::readGrammar(string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return -1;
    }

    string line;
    int rc = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;

        // Print error message if the first symbol is not a non-terminal
        if (!isNonTerminal(line[0])) {
            cout << "The first symbol in the line is not a non-terminal" << endl;
            rc = -1;
            break;
        }

        // Print error message if the grammar is not in the correct format
        if (line[1] != ' ' || line[2] != '-' || line[3] != '>') {
            cout << "The grammar is not in the correct format" << endl;
            rc = -1;
            break;
        }

        char lhs = line[0];

        // Save the first symbol as the start symbol 
        // if it is the first production
        if (startProduction == nullptr) {
            startSymbol = lhs;
        }

        bool isNewProduction = true;
        // If there is production with the same lhs symbol
        // Set the current production to that production
        Production *currentProduction = startProduction;
        while (currentProduction != nullptr) {
            if (currentProduction->lhs == lhs) {
                isNewProduction = false;
                break;
            }
            currentProduction = currentProduction->nextProduction;
        }

        node *prev = nullptr;
        // if the production with lhs is not found, create a new production
        if (isNewProduction) {
            currentProduction = new Production(lhs);
            // Create nodes for the right-hand side of the production
        } else {
            // If the production with the same lhs is found
            prev = currentProduction->rhs;
            while (prev->next != nullptr) {
                prev = prev->next;
            }

            if(prev->next == nullptr){
                node *newNode1 = new node(' ');
                node *newNode2 = new node('|');
                prev->next = newNode1;
                newNode1->next = newNode2;
                prev = newNode2;
            }
        }

        // As first 4 symbols will be lhs and '->' so start from 4
        // Save each symbol in the rhs as a node
        size_t i = 4;
        string currentRhs;

        while (i < line.length()) {
            char sym = line[i];

            if (sym == '|' || sym == ';') {
                // Add the accumulated string to the set
                cout << currentRhs << endl;
                currentProduction->rhsSet.insert(currentRhs);
                currentRhs.clear(); // Clear the string for the next RHS
            } else {
                currentRhs += sym; // Accumulate characters into the string
            }

            node *newNode = new node(sym);
            if (prev == nullptr) {
                currentProduction->rhs = newNode;
            } else {
                prev->next = newNode;
            }
            prev = newNode;
            i++;
        }

        // Add the new production to the list of productions
        if (startProduction == nullptr) {
            startProduction = currentProduction;
        } else if(!isNewProduction){
            continue;        
        } else {
            Production *current = startProduction;
            while (current->nextProduction != nullptr) {
                current = current->nextProduction;
            }
            current->nextProduction = currentProduction;
        }
        
        
    }

    file.close();
    return rc;
}

//******************************************************************************
// Function to print the grammar
void Grammar::printGrammar() {
    Production *currentProduction = startProduction;

    while (currentProduction != nullptr) {
        // Print lhs symbol and '->'
        cout << currentProduction->lhs << " -> ";

        // // Print the rhs symbols
        // node *currentNode = current->rhs;
        // while (currentNode != nullptr) {
        //     cout << currentNode->symbol;
        //     currentNode = currentNode->next;
        // }

        // Print the rhs set
        for (string rhsString : currentProduction->rhsSet) {
            cout << rhsString;
            // If the rhs is not the last alternative, print '|'
            if (rhsString != currentProduction->rhsSet.rbegin()->c_str()) {
                cout << " |";
            }
        }

        cout << ';' << endl;

        // Move to the next production
        currentProduction = currentProduction->nextProduction;
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
