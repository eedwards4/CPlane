//
// Created by Ethan Edwards on 10/11/2024.
//

#ifndef CPLANE_SYMBOLS_H
#define CPLANE_SYMBOLS_H

#include <string>


class Symbol {
public:
  int data;
  Symbol* next;

  // Default constructor
  Symbol() {
    std::string IDENT_NAME;                                                                                                                                         
    int IDENT_TYPE;                                                                                                                                                 
    int DATATYPE = -1;                                                                                                                                              
    bool IS_ARRAY = false;                                                                                                                                           
    int ARRAY_SIZE = 0;                                                                                                                                              
    int SCOPE = 1;

    next = NULL;
  }

  // Constructor
  Symbol(std::string IDENT_NAME, int IDENT_TYPE, int DATATYPE, bool IS_ARRAY, int ARRAY_SIZE, int SCOPE) {
    this->IDENT_NAME = IDENT_NAME;
    this->IDENT_TYPE = IDENT_TYPE;
    this->DATATYPE = DATATYPE;
    this->IS_ARRAY = IS_ARRAY;
    this->ARRAY_SIZE = ARRAY_SIZE;
    this->SCOPE = SCOPE;

    this->next = NULL;
  }
};

class SymbolTable {
  Symbol *temp = head;
  
public:
  Symbols() {
    head = NULL;
  }

  void insertAtHead(std::string IDENT_NAME, int IDENT_TYPE, int DATATYPE, bool IS_ARRAY, int ARRAY_SIZE, int SCOPE) {

    // Create the new Node
    Symbol *newSymbol = new Symbol(IDENT_NAME, IDENT_TYPE, DATATYPE, IS_ARRAY, ARRAY_SIZE, SCOPE);

    // First
    if (head == NULL) {
        head = newSymbol;
	return;
    }

    // Insert the newly created linked list at the head
    newSymbol->next = this->head;
    this->head = newSymbol;
  }
  
};



/*
class symbol {
public:
    symbol() = default;
    ~symbol() = default;

private:
    std::string IDENT_NAME;
    int IDENT_TYPE;
    int DATATYPE = -1;
    bool IS_ARRAY = false;
    int ARRAY_SIZE = 0;
    int SCOPE = 1;

};

class symbols {

};
*/

#endif //CPLANE_SYMBOLS_H
