
//
// Created by Ethan Edwards on 10/11/2024.
// Edited by Logan Puntous on 10/22/2024.
//

#include "symbols.h"


/********************                                                                                                                                               
  CLASS: SYMBOL                                                                                                                                                
*********************/

// Default constructor
Symbol::Symbol() {
  std::string IDENT_NAME;
  int IDENT_TYPE;
  int DATATYPE = -1;
  bool IS_ARRAY = false;
  int ARRAY_SIZE = 0;
  int SCOPE = 1;

  next = NULL;
}

// Constructor                                                                                                                                                      
Symbol::Symbol(std::string IDENT_NAME, int IDENT_TYPE, int DATATYPE, bool IS_ARRAY, int ARRAY_SIZE, int SCOPE) {
  this->IDENT_NAME = IDENT_NAME;
  this->IDENT_TYPE = IDENT_TYPE;
  this->DATATYPE = DATATYPE;
  this->IS_ARRAY = IS_ARRAY;
  this->ARRAY_SIZE = ARRAY_SIZE;
  this->SCOPE = SCOPE;

  this->next = NULL;
}




/********************
  CLASS: SYMBOLTABLE
*********************/

// Deconstructing the LL
SymbolTable::~SymbolTable() {
    Symbol *current = head;
    Symbol *nextNode;

    while (current != NULL) {
        nextNode = current->next;
        delete current;
        current = nextNode;
    }
}


// Adding new symbols                                                                                                                                               
void SymbolTable::insertAtHead(std::string IDENT_NAME, int IDENT_TYPE, int DATATYPE, bool IS_ARRAY, int ARRAY_SIZE, int SCOPE) {

  // Create the new Node                                                                                                                                            
  Symbol *newSymbol = new Symbol(IDENT_NAME, IDENT_TYPE, DATATYPE, IS_ARRAY, ARRAY_SIZE, SCOPE);

  // First Symbol
  if (head == NULL) {
    head = newSymbol;
    return;
  }


  newSymbol->next = this->head;

  this->head = newSymbol;
}



// Prints the linked list sequentially                                                                                                                              
void SymbolTable::print() {
  Symbol *temp = head;

  // Check for empty list                                                                                                                                           
  if (head == NULL) {
    // NEED ERROR HERE
    std::cout << "Error in SymbolTable::print(), List is empty." << std::endl;
    return;
  }

  // Traverse the list                                                                                                                                              
  while (temp != NULL) {
    std::cout << "IDENT_NAME: " << temp->IDENT_NAME << std::endl;
    std::cout << "IDENT_TYPE: " << temp->IDENT_TYPE << std::endl;
    std::cout << "DATATYPE: " << temp->DATATYPE << std::endl;
    std::cout << "IS_ARRAY: " << temp->IS_ARRAY << std::endl;
    std::cout << "ARRAY_SIZE: " << temp->ARRAY_SIZE << std::endl;
    std::cout << "SCOPE: " << temp->SCOPE << std::endl;
    std::cout << std::endl; // Spacing for readability
    temp = temp->next;
  }
}
