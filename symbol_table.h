//
// Created by Ethan Edwards on 10/24/2024.
// Edited by Logan Puntous on 10/29/2024.
//

#ifndef CPLANE_SYMBOL_TABLE_H
#define CPLANE_SYMBOL_TABLE_H

#include <string>
#include <vector>

#include "errors.h"
#include "tokens.h"
#include "symbols.h"
#include "exec_path.h"

class symbol_node {
public:
    // Constructor
    symbol_node() = default; // Default constructor

    // Getters
    std::string getIDENT_NAME() { return IDENT_NAME; }
    int getIDENT_TYPE() const { return IDENT_TYPE; }
    int getDATATYPE() const { return DATATYPE; }
    bool getIS_ARRAY() const { return IS_ARRAY; }
    int getARRAY_SIZE() const { return ARRAY_SIZE; }
    bool getIS_PARAMETER() const { return IS_PARAMETER; }
    std::string getPARAMETER_FOR() const { return PARAMETER_FOR; }
    int getSCOPE() const { return SCOPE; }
    int get_line() const { return line; }
    int get_column() const { return column; }

    // Setters
    void setNext(symbol_node* nextSymbol) { next = nextSymbol; }
    void setIDENT_NAME(const std::string& name) { IDENT_NAME = name; }
    void setIDENT_TYPE(int type) { IDENT_TYPE = type; }
    void setDATATYPE(int dataType) { DATATYPE = dataType; }
    void setIS_ARRAY(bool isArray) { IS_ARRAY = isArray; }
    void setARRAY_SIZE(int arraySize) { ARRAY_SIZE = arraySize; }
    void setIS_PARAMETER(bool isParameter) { IS_PARAMETER = isParameter; }
    void setPARAMETER_FOR(const std::string& functionName) { PARAMETER_FOR = functionName; }
    void setSCOPE(int scope) { SCOPE = scope; }
    void set_line(int lineNum) { line = lineNum; }
    void set_column(int columnNum) { column = columnNum; }

    void set_params(symbol_node* paramList) { params = paramList; }

    // Navigation
    symbol_node* getNext() { return next; }
    symbol_node* get_params() { return params; }

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
    symbol_node* next;
    symbol_node* params; // Pointer to the head of the parameter list
    // Errors
    int line = -1;
    int column = -1;
};







class symbol_table {
public:
    // Constructor
    symbol_table() = default;
    ~symbol_table();

    // Setters
    void generateSymbolTable(exec_path& path);
    exec_node* addVariables(exec_node* current, int dataType = -1, int scope = -1);
    exec_node* addParameters(symbol_node* functionSymbol, exec_node* current);

    // Getters
    void printSymbolTable(const std::string& filename) const; // Function to print all symbols in the table

private:
    // Setup
    void addSymbol(symbol_node* newSymbol); // Helper function to add a symbol to the table
    // Navigation
    std::vector<symbol_node*> scopes; // Vector of head pointers to different scopes
    bool global_scope = false; // Flag to indicate if variables have been defined in the global scope
};


#endif //CPLANE_SYMBOL_TABLE_H
