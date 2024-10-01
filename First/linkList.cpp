#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "linkList.h"

using namespace std;

//******************************************************************************
node::node() {
    symbol = 0;
    prev = nullptr;
    next = nullptr;
}

//******************************************************************************
node::node(char sym) {
    symbol = sym;
    prev = nullptr;
    next = nullptr;
}

//******************************************************************************
node::~node() {
    prev = nullptr;
    next = nullptr;
}

//******************************************************************************
LinkList::LinkList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

//******************************************************************************
LinkList::~LinkList() {
    node *currentNode = head;
    while (currentNode != nullptr) {
        node *nextNode = currentNode->next;
        delete currentNode;
        currentNode = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

//******************************************************************************
void LinkList::insert(char sym) {
    // If sym already exists in the list, do not insert
    if (search(sym)) {
        return;
    }

    node *newNode = new node(sym);
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
}

//******************************************************************************
void LinkList::remove(char sym) {
    node *currentNode = head;
    while (currentNode != nullptr) {
        if (currentNode->symbol == sym) {
            if (currentNode->prev != nullptr) {
                currentNode->prev->next = currentNode->next;
            } else {
                head = currentNode->next;
            }
            if (currentNode->next != nullptr) {
                currentNode->next->prev = currentNode->prev;
            } else {
                tail = currentNode->prev;
            }
            size--;
            delete currentNode;
            break;
        }
        currentNode = currentNode->next;
    }
}

//******************************************************************************
void LinkList::print() {
    node *currentNode = head;
    while (currentNode != nullptr) {
        cout << currentNode->symbol << " ";
        currentNode = currentNode->next;
    }
}

//******************************************************************************
void LinkList::clear() {
    node *currentNode = head;
    while (currentNode != nullptr) {
        node *nextNode = currentNode->next;
        delete currentNode;
        currentNode = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

//******************************************************************************
int LinkList::getSize() {
    return size;
}

//******************************************************************************
bool LinkList::search(char sym) {
    bool rc = false;
    node *currentNode = head;
    while (currentNode != nullptr) {
        if (currentNode->symbol == sym) {
            rc = true;
        }
        currentNode = currentNode->next;
    }
    return rc;
}