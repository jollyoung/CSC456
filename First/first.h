#ifndef FIRST_H
#define FIRST_H

#define MAX 10
using namespace std;


// Structure to store the grammar
class Grammar {
    private:
        char production[MAX][MAX];
        char first;
        int numProductions;
    public:
        void calculateFirst(char c);
        void readGrammar(const string& filename);
};

//int list class
class intList{
    private:
    int *a;
    int capacity;
    int MAX;
    public:
    intList(int MAX = 10);
    ~intList();
};

#endif
