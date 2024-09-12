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
    int rc = 0;
    string line;
    ifstream file(filename);

    // Print error message if the file is not found
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        rc = -1;
    }

    // Read the file line by line
    while (getline(file, line)) {
        // Skip empty lines
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

        // Save the lhs symbol
        char lhs = line[0];

        // Save the first symbol as the start symbol 
        // if it is the first production
        if (startProduction == nullptr) {
            startSymbol = lhs;
        }

        // Check if the production with the same lhs symbol exists
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

        // Create a new node for location of rhs
        node *rhsNode = nullptr;

        // If the production with lhs is not found, create a new production
        // And set it as the current production
        if (isNewProduction) {
            currentProduction = new Production(lhs);
        } else {
            // If the production with the same lhs is found
            // Move current rhsNode to the end of the rhs of the production
            rhsNode = currentProduction->rhs;
            while (rhsNode->next != nullptr) {
                rhsNode = rhsNode->next;
            }

            // If we are adding a new alternative
            // Add a new node with ' |' symbol at the end of the rhs
            if(rhsNode->next == nullptr){
                node *newNode1 = new node(' ');
                node *newNode2 = new node('|');
                rhsNode->next = newNode1;
                newNode1->next = newNode2;
                rhsNode = newNode2;
            }
        }

        // As first 4 symbols will be lhs and '->' so start from 4
        size_t i = 4;
        string currentRhs;

        while (i < line.length() && rc == 0) {
            char sym = line[i];

            // If the symbol is a terminal, non-terminal or epsilon
            // Print error message and break the loop
            if (isTerminal(sym) || 
                isNonTerminal(sym) || 
                sym == '&' || 
                sym == '|' || 
                sym == ';' || 
                isspace(sym)) {
                rc = 0;
            } else {
                cout << "Symbol " << sym << " in grammar is invalid" << endl;
                rc = -1;
                break;
            }

            // Create a new node for the symbol
            node *newNode = new node(sym);
            if (rhsNode == nullptr) {
                currentProduction->rhs = newNode;
            } else {
                rhsNode->next = newNode;
            }
            rhsNode = newNode;

            // Save symbols in a string to add to the set of rhs
            if (sym == '|' || sym == ';') {
                if (sym == ';'){
                    currentRhs += ' ';
                }
                // Add the accumulated string to the set
                currentProduction->rhsSet.insert(currentRhs);

                // Clear the string for the next rhs
                currentRhs.clear(); 
            } else {
                // Accumulate characters into the string
                currentRhs += sym; 
            }

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
    
        if (rc == -1){
            cout << "Error in grammar" << endl;
            break;
        }
    }

    file.close();

    

    return rc;
}

//******************************************************************************
// Function to print the grammar
void Grammar::printGrammar() {
    cout << "Grammar:" << endl;

    // Set the current production to start from the first production
    Production *currentProduction = startProduction;

    while (currentProduction != nullptr) {
        // Print lhs symbol and '->'
        cout << currentProduction->lhs << " -> ";

        // Print the rhs set
        for (string rhsString : currentProduction->rhsSet) {
            cout << rhsString;

            // If the rhs is not the last alternative, print '|'
            if (rhsString != currentProduction->rhsSet.rbegin()->c_str()) {
                cout << "|";
            }
        }

        // Print the last symbol of the production
        cout << ';' << endl;

        // Move to the next production
        currentProduction = currentProduction->nextProduction;
    }

    cout << endl;
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
void Grammar::printAll() {
    printGrammar();
    printStartSymbol();
    printTerminals();
    printNonTerminals();
    cout << endl;
}

// //******************************************************************************
// bool Grammar::calculateFirst(char symbol) {
//     bool rc = false;

//     cout << "Calculating First(" << symbol << ")" << endl;

//     // If the symbol is a terminal or epsilon and exists in the set of terminals
//     if (isTerminal(symbol)) {
//         if (terminals.find(symbol) != terminals.end()) {
//             firstSets.insert(symbol);
//             rc = true;
//         } else {
//             cout << "Terminal " << symbol << " not found in the set of terminals" << endl;
//         }
//     } else if (isNonTerminal(symbol)) {
//         // If the symbol is a non-terminal, but not in the set of non-terminals
//         if (nonTerminals.find(symbol) == nonTerminals.end()) {
//             cout << "Non-terminal " << symbol << " not found in the set of non-terminals" << endl;
//         } else {
//             Production *currentProduction = startProduction;
//             while (currentProduction != nullptr) {
//                 // Find production with the given non-terminal in the lhs
//                 if (currentProduction->lhs == symbol) {
//                     node *currentNode = currentProduction->rhs;
//                     while (currentNode != nullptr) {
//                         cout << "Current node: " << currentNode->symbol << endl;
//                         cout << "First set: ";
//                         for (char first : firstSets) {
//                             cout << first << " ";
//                         }
//                         cout << endl;

//                         if (isTerminal(currentNode->symbol)) {
//                             firstSets.insert(currentNode->symbol);
//                             rc = true;
//                             break;
//                         } else if (isNonTerminal(currentNode->symbol)) {
//                             // Recursive call for non-terminal
//                             if (calculateFirst(currentNode->symbol)) {
//                                 rc = true;
//                             }
//                         } else if (currentNode->symbol == '&') {
//                             firstSets.insert('&');
//                             rc = true;
//                             break;
//                         }

//                         // Move to the next alternative
//                         while (currentNode != nullptr && currentNode->symbol != '|') {
//                             currentNode = currentNode->next;
//                         }
//                         if (currentNode != nullptr) {
//                             currentNode = currentNode->next;
//                         }
//                     }
//                 }
//                 currentProduction = currentProduction->nextProduction;
//             }
//         }
//     } else {
//         // If the symbol is not a terminal or non-terminal
//         cout << "Symbol " << symbol << " is not a terminal or non-terminal" << endl;
//     }

//     return rc;
// }

// //******************************************************************************
// // Function to print the first sets
// void Grammar::printFirstSets(char symbol) {
    
//     // If the first set is calculated successfully
//     if(calculateFirst(symbol)){
//         cout << "First(" << symbol << ") = { ";
//         for (char first : firstSets) {
//             cout << first << " ";
//         }
//         cout << "}" << endl;
//     } else {
//         cout << "First(" << symbol << ") is not found" << endl;
//     }

//     // Clear the first set
//     firstSets.clear();
// }
