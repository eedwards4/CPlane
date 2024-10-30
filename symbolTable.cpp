

//
// Created by Ethan Edwards on 10/11/2024.
// Edited by Logan Puntous on 10/22/2024.
//

#include "symbolTable.h"


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
  int line;
  int column;
  next = NULL;
}

// Constructor                                                                                                                                                      
Symbol::Symbol(std::string IDENT_NAME, int IDENT_TYPE, int DATATYPE, bool IS_ARRAY, int ARRAY_SIZE, int SCOPE, int line, int column) {
  this->IDENT_NAME = IDENT_NAME;
  this->IDENT_TYPE = IDENT_TYPE;
  this->DATATYPE = DATATYPE;
  this->IS_ARRAY = IS_ARRAY;
  this->ARRAY_SIZE = ARRAY_SIZE;
  this->SCOPE = SCOPE;
  this->line = line;
  this->column = column;

  this->next = NULL;
}

void Symbol::setNext(Symbol* nextSymbol) {
  next = nextSymbol;
}
void Symbol::setIDENT_NAME(std::string name){
  IDENT_NAME = name;
}
void Symbol::setIDENT_TYPE(int type){
  IDENT_TYPE = type;
}
void Symbol::setDATATYPE(int dataType){
  DATATYPE = dataType;
}
void Symbol::setIS_ARRAY(bool isArray){
  IS_ARRAY = isArray;
}
void Symbol::setARRAY_SIZE(int arraySize){
  ARRAY_SIZE = arraySize;
}
void Symbol::setIS_PARAMETER(bool isParameter){
  IS_PARAMETER = isParameter;
}
void Symbol::setSCOPE(int scope){
  SCOPE = scope;
}
void Symbol::set_line(int Line){
  line = Line;
}
void Symbol::set_column(int Column){
  column = Column;
}
void Symbol::set_params(Symbol* paramList){
  params = paramList;
}




std::string Symbol::getIDENT_NAME() {
  return IDENT_NAME;
}
int Symbol::getIDENT_TYPE() {
  return IDENT_TYPE;
}
int Symbol::getDATATYPE() {
  return DATATYPE;
}
bool Symbol::getIS_ARRAY() {
  return IS_ARRAY;
}
int Symbol::getARRAY_SIZE() {
  return ARRAY_SIZE;
}
bool Symbol::getIS_PARAMETER() {
  return IS_PARAMETER;
}
std::string Symbol::getPARAMETER_FOR() {
  return PARAMETER_FOR;
}
int Symbol::getSCOPE() {
  return SCOPE;
}
int Symbol::get_line() {
  return line;
}
int Symbol::get_column() {
  return column;
}
Symbol* Symbol::getNext() {
  return next;
}
Symbol* Symbol::get_params() {
  return params;
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

/*
// Adding new symbols                                                                                                                                               
void SymbolTable::insertAtHead(std::string IDENT_NAME, int IDENT_TYPE, int DATATYPE, bool IS_ARRAY, int ARRAY_SIZE, int SCOPE, int line, int column) {

// Create the new Node                                                                                                                                            
Symbol *newSymbol = new Symbol(IDENT_NAME, IDENT_TYPE, DATATYPE, IS_ARRAY, ARRAY_SIZE, SCOPE, line, column);

// First Symbol
if (head == NULL) {
head = newSymbol;
return;
}
newSymbol->next = this->head;
this->head = newSymbol;
}
*/

void SymbolTable::generateSymbolTable(exec_path &path) {
  exec_node* current = path.get_head(); // Get the head of the execution path
  int scope = 0; // Initialize the scope level at global (0)
  int function_scope = 0; // Track the function scope level

  while (current != nullptr) {
    if (current->get_type() == tokens::TOKEN_AS_STRING) {
      std::string value = current->get_value();

      if (value == "function" || value == "procedure") {
	handleFunctionOrProcedure(current, value, function_scope);
	current = addParameters(current);
      } 
      else {
	int dataType = symbols::data_types::check_type(value);
	if ( dataType != -1){
	  current = addVariables(current, dataType, scope);
	}
      }
    }

    // Update scope based on braces
    if (current->get_next() == nullptr && current->get_fold() != nullptr) {
      current->get_type() == tokens::OPEN_BRACE ? scope++ : scope--;
      current = current->get_fold();
    } else {
      current = current->get_next();
    }
  }
}

void SymbolTable::addSymbol(Symbol* newSymbol) {
  bool global_scope = false; // Track if we're in the global scope

  if (newSymbol->getSCOPE() == 0) {
    global_scope = true; // We have a globally scoped variable

    if (!scopes.empty() && scopes[0]->getIDENT_TYPE() != symbols::identifiers::DATATYPE) {
      // Prepend global variables if they are defined after the first function/procedure
      scopes.insert(scopes.begin(), newSymbol);
      return;
    }
  }

  if (scopes.empty()) { // First symbol in the table
    scopes.push_back(newSymbol);
  } else {
    int scopesSize = global_scope ? scopes.size() - 1 : scopes.size();
        
    if (newSymbol->getSCOPE() > scopesSize) { // New scope
      if (newSymbol->getIDENT_TYPE() == symbols::identifiers::DATATYPE) {
	// Non-functions/Non-procedures cannot be declared as a unique scope
	errors::E_NON_FUNCTION_SCOPE_DECLARATION(newSymbol->get_line(), newSymbol->get_column(), newSymbol->getIDENT_NAME());
      }
      scopes.push_back(newSymbol);
    } else {
      int newScope = global_scope ? newSymbol->getSCOPE() : newSymbol->getSCOPE() - 1;
      Symbol* current = scopes[newScope];

      // Check for duplicates in the current scope
      while (current != nullptr) {
	if (current->getIDENT_NAME() == newSymbol->getIDENT_NAME()) {
	  // Handle duplicate symbol error
	  errors::E_ALREADY_DEFINED_VARIABLE_LOCAL(newSymbol->get_line(), newSymbol->get_column(), newSymbol->getIDENT_NAME());
	}
	if (current->getNext() == nullptr) break; // If we are at the end of the list
	current = current->getNext();
      }

      // Check for duplicates in function parameters
      if (scopes[newScope]->get_params() != nullptr) {
	Symbol* paramCurrent = scopes[newScope]->get_params();
	while (paramCurrent != nullptr) {
	  if (paramCurrent->getIDENT_NAME() == newSymbol->getIDENT_NAME()) {
	    // Handle duplicate symbol error
	    errors::E_ALREADY_DEFINED_VARIABLE_LOCAL(newSymbol->get_line(), newSymbol->get_column(), newSymbol->getIDENT_NAME());
	  }
	  paramCurrent = paramCurrent->getNext();
	}
      }

      // Check for duplicates in global scope
      if (global_scope) {
	Symbol* g_current = scopes[0];
	while (g_current != nullptr) {
	  if (g_current->getIDENT_NAME() == newSymbol->getIDENT_NAME()) {
	    // Handle duplicate symbol error
	    errors::E_ALREADY_DEFINED_VARIABLE_GLOBAL(newSymbol->get_line(), newSymbol->get_column(), newSymbol->getIDENT_NAME());
	  }
	  g_current = g_current->getNext();
	}
      }

      current->setNext(newSymbol); // Add the new symbol to the end of the current scope
    }
  }
}



  
void SymbolTable::handleFunctionOrProcedure(exec_node* &current, const std::string &type, int &function_scope) {
  if (function_scope > 0) {
    //std::cout << "here" << std::endl;
    errors::E_NESTED_FUNCTION_NOT_ALLOWED(current->get_line(), current->get_column(), current->get_next()->get_next()->get_value());
  }

  //std::string IDENT_NAME, int IDENT_TYPE, int DATATYPE, bool IS_ARRAY, int ARRAY_SIZE, int SCOPE
  
  auto newSymbol = new Symbol();
  newSymbol->setIDENT_TYPE(type == "function" ? symbols::identifiers::FUNCTION : symbols::identifiers::PROCEDURE);
  newSymbol->setDATATYPE(type == "function" ? symbols::data_types::check_type(current->get_next()->get_value()) : symbols::data_types::NA);
  newSymbol->setIDENT_NAME(current->get_next()->get_next()->get_value());
  newSymbol->setSCOPE(++function_scope);
  newSymbol->set_line(current->get_line());
  newSymbol->set_column(current->get_column());
  
  addSymbol(newSymbol);


  

  current = current->get_next()->get_next()->get_next(); // Move to the next token after the name
}

exec_node* SymbolTable::addVariables(exec_node* current, int dataType, int scope) {
  auto newSymbol = new Symbol();
  newSymbol->setIDENT_TYPE(symbols::identifiers::DATATYPE);
  newSymbol->setDATATYPE(dataType);
  newSymbol->setIDENT_NAME(current->get_value());
  newSymbol->setSCOPE(scope);
  newSymbol->set_line(current->get_line());
  newSymbol->set_column(current->get_column());

  if (current->get_next()->get_type() == tokens::OPEN_BRACKET) {
    newSymbol->setIS_ARRAY(true);
    current = current->get_next()->get_next(); // Move to array size
    newSymbol->setARRAY_SIZE(std::stoi(current->get_value()));
    current = current->get_next(); // Consume closing bracket
  }

  addSymbol(newSymbol);

  while (current->get_type() != ',' && current->get_type() != ';') {
    current = current->get_next();
  }

  if (current->get_type() == ',') {
    current = current->get_next(); // Move to the next variable
    addVariables(current, dataType, scope);
  }

  return current;
}

exec_node* SymbolTable::addParameters(exec_node* current) {
  while (current->get_next() != nullptr) {
    current = current->get_next();
    if (current->get_type() == tokens::TOKEN_AS_STRING && current->get_value() != "void") {
      auto paramSymbol = new Symbol();
      paramSymbol->setIS_PARAMETER(true);
      paramSymbol->setIDENT_TYPE(symbols::identifiers::PARAMETER);
      paramSymbol->setDATATYPE(symbols::data_types::check_type(current->get_value()));
      current = current->get_next();
      paramSymbol->setIDENT_NAME(current->get_value());
      paramSymbol->setSCOPE(0); // Assuming parameters are at a function level

      // Set array properties if necessary
      if (current->get_next()->get_type() == tokens::OPEN_BRACKET) {
	paramSymbol->setIS_ARRAY(true);
	current = current->get_next()->get_next(); // Move to array size
	paramSymbol->setARRAY_SIZE(std::stoi(current->get_value()));
	current = current->get_next(); // Consume rest of the declaration
      }

      // Add line/column info
      paramSymbol->set_line(current->get_line());
      paramSymbol->set_column(current->get_column());
      addSymbol(paramSymbol);
    }
  }
  return current;
}








void SymbolTable::printSymbolTable(std::string& filename) {
  std::ofstream out(filename);
  if (!out) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }
  for (const auto& scope : scopes) {
    Symbol* current = scope;
    while (current != nullptr) {
      out << "    IDENTIFIER_NAME: " << current->getIDENT_NAME() << std::endl;
      out << "    IDENTIFIER_TYPE: " << symbols::identifiers::get_type(current->getIDENT_TYPE()) << std::endl;
      out << "           DATATYPE: " << symbols::data_types::get_type(current->getDATATYPE()) << std::endl;
      if (current->getIS_ARRAY() == 0){
	out << "  DATATYPE_IS_ARRAY: no" << std::endl;
	out << "DATATYPE_ARRAY_SIZE: " << current->getARRAY_SIZE() + 1 << std::endl;
      }
      else{
	out << "  DATATYPE_IS_ARRAY: yes" << std::endl;
	out << "DATATYPE_ARRAY_SIZE: " << current->getARRAY_SIZE() << std::endl;
      }
      out << "              SCOPE: " << current->getSCOPE() << std::endl;
      out << std::endl;
      current = current->getNext();
    }
  }
  // Parameters
  for (const auto& scope : scopes) {
    if (scope->get_params() != nullptr){
      out << " PARAMETER LIST FOR: " << scope->getIDENT_NAME() << std::endl;
      Symbol* paramCurrent = scope->get_params();
      while (paramCurrent != nullptr) {
	out << "    IDENTIFIER_NAME: " << paramCurrent->getIDENT_NAME() << std::endl;
	out << "           DATATYPE: " << symbols::data_types::get_type(paramCurrent->getDATATYPE()) << std::endl;
	if (paramCurrent->getIS_ARRAY() == 0){
	  out << "  DATATYPE_IS_ARRAY: no" << std::endl;
	  out << "DATATYPE_ARRAY_SIZE: " << paramCurrent->getARRAY_SIZE() + 1 << std::endl;
	}
	else{
	  out << "  DATATYPE_IS_ARRAY: yes" << std::endl;
	  out << "DATATYPE_ARRAY_SIZE: " << paramCurrent->getARRAY_SIZE() << std::endl;
	}
	out << "              SCOPE: " << paramCurrent->getSCOPE() << std::endl;
	out << std::endl;
	paramCurrent = paramCurrent->getNext();
      }
    }
  }
}



