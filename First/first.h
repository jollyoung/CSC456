#ifndef FIRST_H
#define FIRST_H

using namespace std;

class node {
    private:
        char symbol;
        // Pointer to the next node
        node *next;
    
    public:
        node();
        node(char sym);
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
        Production();
        Production(char non_terminal) : lhs(non_terminal), rhs(nullptr), nextProduction(nullptr) {};

        friend class Grammar;
};

// Structure to store the grammar
class Grammar {
    private:
        Production *startProduction;
        char startSymbol;

    public:
        Grammar();
        int readGrammar(string& filename);
        void printGrammar();
};

#endif