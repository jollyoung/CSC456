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
#include "linkList.h"

using namespace std;

//******************************************************************************
Production::Production() {
    lhs = 0;
    lhsNode = nullptr;
    rhsNode = nullptr;
    nextProduction = nullptr;
}

//******************************************************************************
Production::Production(char non_terminal) {
    lhs = non_terminal;
    lhsNode = nullptr;
    rhsNode = nullptr;
    nextProduction = nullptr;
}

//******************************************************************************
Production::~Production() {
    node *currentNode = rhsNode;
    while (currentNode != nullptr) {
        node *nextNode = currentNode->next;
        delete currentNode;
        currentNode = nextNode;
    }
    lhsNode = nullptr;
    rhsNode = nullptr;
    nextProduction = nullptr;
}

//******************************************************************************
Grammar::Grammar() {
    startProduction = nullptr;
    startSymbol = 0;
    isEpsilon = false;
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

    // Check the position of ';' end sign of the production
    size_t endPos = string::npos;

    node *currentNode = nullptr;
    node *prevNode = nullptr;
    Production *currentProduction = nullptr;

    char lhs = 0;    
    bool firstLine = true;

    // Read the file line by line
    while (getline(file, line)){

        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        // Check if the current line is the first line of production
        bool productionFL = false;

        // If current line is the first line or
        // the end sign is found in the previous line
        // Create a new production
        if((endPos != string::npos) || firstLine){
            lhs = line[0];
            firstLine = false;

            Production *newProduction = new Production(lhs);
            currentProduction = newProduction;

            // If current line is the first line
            // Save current production as the first production
            if(startProduction == nullptr){
                startProduction = currentProduction;
                startSymbol = lhs;
            } else {
                
                // If current line is not the first line
                // Save the current production to the end of the grammar
                Production *temp = startProduction;
                while(temp->nextProduction != nullptr){
                    temp = temp->nextProduction;
                }
                temp->nextProduction = currentProduction; 
            }   

            productionFL = true;
        }

        // Intiger to keep track of the current position in the line
        size_t i = 0;

        // While the line ends
        while(i < line.size()){
            // If current line is the first line of production
            // Save the current node as the lhs node
            if(productionFL){
                currentProduction->lhsNode = new node(line[i]);
                currentNode = currentProduction->lhsNode;
                productionFL = false;
            } else {
                // If current line is not the first line of production
                // Create a new node for the current symbol
                currentNode = new node(line[i]);
            }

            // If the previous node exists
            if(prevNode != nullptr){
                // Save the current node as the next node of the previous node
                prevNode->next = currentNode;
                // Save the previous node as the previous node of the current node
                currentNode->prev = prevNode;
            }


            // Save the previous node location
            prevNode = currentNode;

            // Break the chain if ';' is encountered
            if (line[i] == ';') {
                currentNode->next = nullptr;
                prevNode = nullptr;
                break;
            }

            i++;
        }

        // Find the position of ';' in the line
        endPos = line.find(';');
        // If the end sign is found
        if(endPos != string::npos){
            // Check ';' locates at the end of the line
            if(endPos != line.size() - 1){
                cerr << "Error: ';' is not at the end of the line" << endl;
                rc = -1;
                break;
            } else {
                // Search node for saving current position within the production
                node *searchNode = currentProduction->lhsNode;

                // Find the position of '->' in the production
                while(searchNode != nullptr){
                    if(searchNode->symbol == '-'){
                        if(searchNode->next->symbol == '>'){
                            break;
                        }
                    }
                    searchNode = searchNode->next;
                }

                // If '->' is not found in the production
                if(searchNode == nullptr){
                    cerr << "Error: '->' is not found in the production" << endl;
                    rc = -1;
                    break;
                } else {
                    // Save the position of '->' in the production
                    currentProduction->rhsNode = searchNode->next->next;
                }
            }
        }

    }

    Production *temp = startProduction;

    // After reading the file, save terminals and non-terminals
    while(temp != nullptr){
        // Since all lhs symbols are non-terminals
        // Insert all lhs symbols to the set of non-terminals
        nonTerminals.insert(temp->lhs);

        temp = temp->nextProduction;
    }

    temp = startProduction;
    while(temp != nullptr){
        // For the case, where the rhs contains non-terminals
        node *searchNode = temp->rhsNode;
        while (searchNode != nullptr) {
            char symbol = searchNode->symbol;
            
            // If the symbol is not a terminal or epsilon or special character
            // Insert the symbol to the set of non-terminals
            if(symbol != '|' && symbol != '&' && isspace(symbol) == 0 && !nonTerminals.search(symbol) && symbol != ';' && symbol != '-' && symbol != '>'){
                terminals.insert(symbol);
            }
            
            searchNode = searchNode->next;
        }
        temp = temp->nextProduction;
    }

    
    
    file.close();

    

    return rc;
}

//******************************************************************************
// Function to print the grammar
void Grammar::printGrammar() {
    cout << "Production:" << endl;

    // Set the current production to start from the first production
    Production *currentProduction = startProduction;

    while (currentProduction != nullptr) {
        // Print lhs symbol and '->'
        cout << currentProduction->lhs << " -> ";

        // Print the rhs symbols
        node *currentNode = currentProduction->rhsNode;
        int spaceCount = 0;  // Variable to count consecutive spaces

        bool newLine = false;
        
        while (currentNode != nullptr) {
            char symbol = currentNode->symbol;
            

            // Check if the symbol is a space
            if (isspace(symbol)) {
                spaceCount++;
            } else {
                // If we encounter a non-space character, reset space count
                spaceCount = 0;
            }

            // If '|' is encountered and there are more than 3 spaces before it
            if (isspace(symbol) && spaceCount > 3 && !newLine) {
                cout << endl;  // Print a newline
                cout << "   ";  // Print 3 spaces
                newLine = true;
            } else {
                // Print the symbol as normal
                cout << symbol;
            }

            currentNode = currentNode->next;
        }

        cout << endl;

        // Move to the next production
        currentProduction = currentProduction->nextProduction;
    }
}


//******************************************************************************
// Function to print all terminals
void Grammar::printTerminals() {
    // Print all terminal symbols
    cout << "Terminals: ";
    terminals.print();
    cout << endl;
}

//******************************************************************************
// Function to print all non-terminals
void Grammar::printNonTerminals() {
    // Print all non-terminal symbols
    cout << "Non-terminals: ";
    nonTerminals.print();
    cout << endl;
}

//******************************************************************************
// Function to print the start symbol
void Grammar::printStartSymbol() {
    cout << "Start symbol: " << startSymbol << endl;
}

//******************************************************************************
void Grammar::printAll() {
    printStartSymbol();
    cout << endl;
    printTerminals();
    cout << endl;
    printNonTerminals();
    cout << endl;
    printGrammar();
    cout << endl;
}

//******************************************************************************
bool Grammar::calculateFirst(char symbol) {
    bool rc = false;
    isEpsilon = false;

    // If the symbol is a terminal or epsilon and exists in the set of terminals
    if (terminals.search(symbol) || symbol == '&') {
        if(symbol == '&'){
            isEpsilon = true;
        }
        firstSets.insert(symbol);
        rc = true;
    } 
    
    else if (nonTerminals.search(symbol)) {
        Production *currentProduction = startProduction;
        while (currentProduction != nullptr) {
            // Find production with the given non-terminal in the lhs
            if (currentProduction->lhs == symbol) {
                node *currentNode = currentProduction->rhsNode;

                while (currentNode != nullptr) {
                    
                    // Skip spaces
                    while(isspace(currentNode->symbol)){
                        currentNode = currentNode->next;
                    }

                    // If the symbol is a terminal, save it to the first set
                    if (terminals.search(currentNode->symbol)) {
                        firstSets.insert(currentNode->symbol);
                        rc = true;
                    } 
                    
                    else if (currentNode->symbol == '&') {
                        isEpsilon = true;
                        firstSets.insert(currentNode->symbol);
                        rc = true;
                    } 
                    
                    else if (nonTerminals.search(currentNode->symbol)) {
                        // Recursive call for non-terminal
                        rc = calculateFirst(currentNode->symbol);
                        
                        if(isEpsilon && currentNode->next != nullptr){
                            char nextSym = currentNode->next->symbol;
                            if(terminals.search(nextSym)){
                                firstSets.remove('&');
                                firstSets.insert(nextSym);
                            } else if(nonTerminals.search(nextSym)){
                                firstSets.remove('&');
                                calculateFirst(nextSym);
                            }
                        }
                    }


                    // Move to the next alternative
                    while (currentNode != nullptr && currentNode->symbol != '|') {
                        currentNode = currentNode->next;
                    }
                    if (currentNode != nullptr) {
                        currentNode = currentNode->next;
                    }
                }
            }
            currentProduction = currentProduction->nextProduction;
        }
        
    } else {
        // If the symbol is not a terminal or non-terminal
        cout << "Symbol " << symbol << " is not a terminal or non-terminal" << endl;
    }

    return rc;
}

//******************************************************************************
// Function to calculate the first set of multiple symbols
bool Grammar::calculateFirst(string symbols) {
    bool rc = false;

    // For each symbol in the input string
    for (size_t i = 0; i < symbols.size(); i++) {
        // Calculate the first set of the symbol
        rc = calculateFirst(symbols[i]);
        if(rc){
            break;
        }
    }
    
    return rc;
}

//******************************************************************************
// Function to print the first sets
void Grammar::printFirstSets(char symbol) {
    
    // If the first set is calculated successfully
    if(calculateFirst(symbol)){
        cout << "First(" << symbol << ") = { ";
        firstSets.print();
        cout << "}" << endl;
    } else {
        cout << "First(" << symbol << ") is not found" << endl;
    }

    // Clear the first set
    firstSets.clear();
}

//******************************************************************************
// Function to print the first sets
void Grammar::printFirstSets(string symbols) {
    
    // If the first set is calculated successfully
    if(calculateFirst(symbols)){
        cout << "First(" << symbols << ") = { ";
        firstSets.print();
        cout << "}" << endl;
    } else {
        cout << "First(" << symbols << ") is not found" << endl;
    }

    // Clear the first set
    firstSets.clear();
}

//******************************************************************************
void Grammar::printFirstAll(){
    char symbol;

    cout << "First sets for all symbols:" << endl;

    cout << "Non-terminals:" << endl;
    for (int i = 0; i < nonTerminals.getSize(); i++) {
        symbol = nonTerminals.getSymbol(i);
        printFirstSets(symbol);
    }

    cout << endl;

    cout << "Terminals:" << endl;
    for (int i = 0; i < terminals.getSize(); i++) {
        symbol = terminals.getSymbol(i);
        printFirstSets(symbol);
    }
}