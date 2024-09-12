/*
* Rolling Pandas
* CSC453 Fall 2024
* Assignment: First part 1
*/

#ifndef FIRST_H
#define FIRST_H

#include <string>
#include <set>

using namespace std;

// Node class
// Save the symbol and a pointer to the next node
class node {
    private:
        char symbol;
        node *next;
    
    public:
        node();
        // Constructur to create a node with a symbol
        node(char sym);
        ~node();

        friend class Production;
        friend class Grammar;
};

// Production class
// Save the left-hand side, the right-hand side and a pointer to the next production
class Production {
    private:
        // Save non-terminal symbol in the left-hand side
        char lhs;
        // Save the location of the first node in the right-hand side
        node *rhs;
        // Set to save rhs without parsing
        set<string> rhsSet;
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
        // Set to save all terminals
        set<char> terminals;
        // Set to save all non-terminals
        set<char> nonTerminals;
        // Set to save firsts
        set<char> firstSets;

        // Function to check if a symbol is a non-terminal
        bool isNonTerminal(char symbol); 
        bool isTerminal(char symbol);

        // Function to calculate the first set of an input symbol
        bool calculateFirst(char symbol);

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
        void printFirstSets(char symbol);
};

#endif