//
// Created by Ethan Edwards on 10/24/2024.
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

    // Parameters
    std::string IDENT_NAME;
    int IDENT_TYPE = -1;
    int DATATYPE = -1;
    bool IS_ARRAY = false;
    int ARRAY_SIZE = -1;
    bool IS_PARAMETER = false;
    std::string PARAMETER_FOR; // What function does this parameter belong to
    int SCOPE_GLOBAL = -1; // Global scope (i.e. what function this symbol belongs to)
    // int SCOPE_LOCAL = -1; // Local scope (i.e. what block this symbol belongs to)
    // ^ Currently unimplemented, but will be needed in future

    // Getters
    symbol_node* get_next() { return next; }
    symbol_node* get_params() { return params; }
    int get_line() const { return line; }
    int get_column() const { return column; }

    // Setters
    void set_next(symbol_node* nextSymbol) { next = nextSymbol; }
    void set_params(symbol_node* paramList) { params = paramList; }
    void set_line(int lineNum) { line = lineNum; }
    void set_column(int columnNum) { column = columnNum; }

private:
    // Protected
    // Navigation
    symbol_node* next = nullptr;
    symbol_node* params = nullptr;

    // Error tracking
    int line = -1;
    int column = -1;
};

class symbol_table {
public:
    // Constructor
    symbol_table() = default;
    ~symbol_table();

    // Setters
    void build_table(exec_path& path);
    exec_node* var_handler(exec_node* current, int dType, int scope, int local_scope);
    exec_node* param_handler(exec_node* current, symbol_node* functionSymbol);

    // Getters
    void print_symbols(const std::string& filename) const;
    bool find_symbol(const std::string name) const;

private:
    // Setup
    void add_symbol(symbol_node* new_symbol);

    // Navigation
    std::vector<symbol_node*> scopes; // Vector of head pointers to different scopes
};


#endif //CPLANE_SYMBOL_TABLE_H
