#ifndef LINKLIST_H
#define LINKLIST_H

// Node class
// Save the symbol and a pointer to the next node
class node {
    private:
        char symbol;
        node *prev;
        node *next;
    
    public:
        node();
        // Constructur to create a node with a symbol
        node(char sym);
        ~node();

        friend class Production;
        friend class Grammar;
        friend class LinkList;
};

class LinkList {
    private:
        node *head;
        node *tail;
        int size;
    
    public:
        LinkList();
        ~LinkList();
        void insert(char sym);
        void print();
        void clear();
        int getSize();
        bool search(char sym);
        void remove(char sym);

        char getSymbol(int index);
};

#endif