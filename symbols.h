//
// Created by Ethan Edwards on 10/11/2024.
// Edited by Logan Puntous on 10/22/2024.
//

#ifndef CPLANE_SYMBOLS_H
#define CPLANE_SYMBOLS_H

#include <string>


class Symbol {
public:
  std::string IDENT_NAME;
  int IDENT_TYPE;
  int DATATYPE;
  bool IS_ARRAY;
  int ARRAY_SIZE;
  int SCOPE;

  Symbol *next;

  // Default constructor
  Symbol();
  
  // Constructor
  Symbol(std::string IDENT_NAME, int IDENT_TYPE, int DATATYPE, bool IS_ARRAY, int ARRAY_SIZE, int SCOPE);
private:
  
};








class SymbolTable {
  Symbol *head;
  
public:
  // Constructor
  SymbolTable() {
    head = NULL;
  }
  
  ~SymbolTable();
  
  void insertAtHead(std::string IDENT_NAME, int IDENT_TYPE, int DATATYPE, bool IS_ARRAY, int ARRAY_SIZE, int SCOPE);
  void print();
};


#endif //CPLANE_SYMBOLS_H
