/*
* Rolling Pandas
* CSC453 Fall 2024
* Assignment: First part 1
*/

#ifndef FIRST_H
#define FIRST_H

#include <string>
#include "linkList.h"

using namespace std;

// Production class
// Save the left-hand side, the right-hand side and a pointer to the next production
class Production {
    private:
        // Save non-terminal symbol in the left-hand side
        char lhs;
        // Save the location of the first node in the left-hand side
        node *lhsNode;
        // Save the location of the first node in the right-hand side
        node *rhsNode;
        // Pointer to the next production
        Production *nextProduction;
    
    public:
        Production();
        // Constructor to create a production with a non-terminal symbol in lhs
        Production(char non_terminal);
        ~Production();

        friend class Grammar;
};

// Grammar class
// Entire grammar is saved in this class
class Grammar {
    private:
        // Pointer to the first production
        Production *startProduction;
        // Start symbol of the grammar
        char startSymbol;
        // Linked list to save non-terminal symbols
        LinkList nonTerminals;
        // Linked list to save terminal symbols
        LinkList terminals;
        // Linked list to save first sets
        LinkList firstSets;

    public:
        Grammar();
        ~Grammar();
        // Function to read the grammar from a text file
        int readGrammar(string& filename);

        // Functions to print
        void printGrammar();
        void printTerminals();
        void printNonTerminals();
        void printStartSymbol();
        void printAll();

        // Function to calculate the first set of an input symbol
        bool calculateFirst(char symbol);
        
        void printFirstSets(char symbol);
};

#endif