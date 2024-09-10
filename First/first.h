#ifndef FIRST_H
#define FIRST_H

using namespace std;

class node {
    private:
        char symbol;
        // Pointer to the next node
        node *next;
        // Pointer to the next non-terminal
        node *nextNT;
    
    public:
        node(char sym) : symbol(sym), next(nullptr), nextNT(nullptr) {}

        friend class Grammar;
};

class Production{
    private:
        char lhs;
        // Start node of right hand side of the production
        node *rhs;
        // Pointer to the next production
        Production *nextProduction;
    
    public:
        Production(char non_terminal) : lhs(non_terminal), rhs(nullptr), nextProduction(nullptr) {};

        friend class Grammar;
};

// Structure to store the grammar
class Grammar {
    private:
        Production *startProduction;
        char startSymbol;

    public:
        Grammar() : startProduction(nullptr), startSymbol(0) {};
        int readGrammar(string& filename);
        void printGrammar();
};

#endif