#ifndef FIRST_H
#define FIRST_H

#include <string>
#include <set>

using namespace std;

class node {
    private:
        char symbol;
        node *next;
        node *nextNT;
    
    public:
        node();
        node(char sym);

        friend class Grammar;
};

class Production {
    private:
        char lhs;
        node *rhs;
        Production *nextProduction;
    
    public:
        Production();
        Production(char non_terminal);

        friend class Grammar;
};

class Grammar {
    private:
        Production *startProduction;
        char startSymbol;

        bool isNonTerminal(char symbol);  // Helper function to identify non-terminals
        bool isTerminal(char symbol);

    public:
        Grammar();
        int readGrammar(string& filename);
        void printGrammar();
        void printTerminals();
        void printNonTerminals();
        void printStartSymbol();
};

#endif
