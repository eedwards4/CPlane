
//
// Created by Ethan Edwards on 10/11/2024.
// Edited by Logan Puntous on 10/22/2024.
//

#ifndef CPLANE_SYMBOLTABLE_H
#define CPLANE_SYMBOLTABLE_H


#include <string>
#include <vector>

#include <iostream>
#include "errors.h"
#include "symbols.h"
#include "exec_path.h"

class Symbol {
public:

  Symbol* next;
  
  // Default constructor
  Symbol();
  
  // Constructor
  Symbol(std::string IDENT_NAME, int IDENT_TYPE, int DATATYPE, bool IS_ARRAY, int ARRAY_SIZE, int SCOPE, int line, int column);

  // Setters
  void setNext(Symbol* nextSymbol);
  void setIDENT_NAME(std::string name);
  void setIDENT_TYPE(int type);
  void setDATATYPE(int dataType);
  void setIS_ARRAY(bool isArray);
  void setARRAY_SIZE(int arraySize);
  void setIS_PARAMETER(bool isParameter);
  void setPARAMETER_FOR(std::string functionName);
  void setSCOPE(int scope);
  void set_line(int lineNum);
  void set_column(int columnNum);
  void set_params(Symbol* paramList);

  // Getters
  
  std::string getIDENT_NAME();
  int getIDENT_TYPE();
  int getDATATYPE();
  bool getIS_ARRAY();
  int getARRAY_SIZE();
  bool getIS_PARAMETER();
  std::string getPARAMETER_FOR();
  int getSCOPE();
  int get_line();
  int get_column();
  Symbol* getNext();
  Symbol* get_params();
  
private:
  std::string IDENT_NAME;
  int IDENT_TYPE;
  int DATATYPE;
  bool IS_ARRAY = false;
  int ARRAY_SIZE = -1;
  bool IS_PARAMETER = false;
  std::string PARAMETER_FOR; // What function does this parameter belong to
  int SCOPE;
  // Navigation
  Symbol* params; // Pointer to the head of the parameter list
  // Errors
  int line = -1;
  int column = -1;


  
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

  void generateSymbolTable(exec_path &path);
  void handleFunctionOrProcedure(exec_node* &current, const std::string &type, int &function_scope);
  void addSymbol(Symbol* newSymbol);
  exec_node* addVariables(exec_node* current, int dataType, int scope);
  exec_node* addParameters(exec_node* current);
  
  void printSymbolTable(std::string& filename);
private:
  std::vector<Symbol*> scopes; // Vector of head pointers to different scopes
  bool global_scope = false; // Flag to indicate if variables have been defined in the global scope
};


#endif //CPLANE_SYMBOLTABLE_H
