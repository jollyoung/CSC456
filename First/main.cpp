#include "first.h"

int main() {
    Grammar grammar;
    string filename = "grammar.txt";
    int rc = grammar.readGrammar(filename);
    if (rc) {
        grammar.printGrammar();
        grammar.printTerminals();
        grammar.printNonTerminals();
        grammar.printStartSymbol();
        grammar.printFirstSets('B');
        grammar.printFirstSets('C');
        grammar.printFirstSets('Z');
        grammar.printFirstSets('b');
        grammar.printFirstSets('*');
    }

    // grammar.~Grammar();

    // filename = "grammar2.txt";
    // rc = grammar.readGrammar(filename);
    // if (rc == 0) {
    //     grammar.printGrammar();
    //     grammar.printTerminals();
    //     grammar.printNonTerminals();
    //     grammar.printStartSymbol();
    //     grammar.printFirstSets('A');
    //     grammar.printFirstSets('C');
    // }
    return 0;
}