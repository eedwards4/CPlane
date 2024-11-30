//
// Created by Ethan Edwards on 10/24/2024.
//

#ifndef CPLANE_SYMBOL_TABLE_H
#define CPLANE_SYMBOL_TABLE_H

#include <string>
#include <vector>
#include <typeinfo>

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
    int get_val_int() const { return val_int; }
    float get_val_float() const { return val_float; }
    char get_val_char() const { return val_char; }
    std::string get_val_string() const { return val_string; }
    bool get_val_bool() const { return val_bool; }

    // Setters
    void set_next(symbol_node* nextSymbol) { next = nextSymbol; }
    void set_params(symbol_node* paramList) { params = paramList; }
    void set_line(int lineNum) { line = lineNum; }
    void set_column(int columnNum) { column = columnNum; }
    void set_val_int(int value) { val_int = value; }
    void set_val_float(float value) { val_float = value; }
    void set_val_char(char value) { val_char = value; }
    void set_val_string(std::string value) { val_string = value; }
    void set_val_bool(bool value) { val_bool = value; }

private:
    // Protected
    // Navigation
    symbol_node* next = nullptr;
    symbol_node* params = nullptr;
    symbol_node* array_children = nullptr;

    // Datatypes
    int val_int;
    float val_float;
    char val_char;
    std::string val_string;
    bool val_bool;
    // Error tracking
    int line = -1;
    int column = -1;
};

class symbol_table {
public:
    // Constructor
    symbol_table() = default;
    symbol_table(ERRORS &in_errors, std::string in_filename){
        errors = in_errors;
        filename = in_filename;
    };

    // Setters
    void build_table(exec_path& path);
    exec_node* var_handler(exec_node* current, int dType, int scope, int local_scope);
    exec_node* param_handler(exec_node* current, symbol_node* functionSymbol);

    // Getters
    void print_symbols(const std::string& filename) const;
    bool find_symbol(const std::string name) const;
    bool update_symbol(const std::string name, const std::string val) const;
    bool is_function(const std::string name) const;
    symbol_node* get_symbol(const std::string name, const int scope);
    int get_function_scope(const std::string name);
    

    ERRORS returnErrors(){
        return errors;
    };

private:
    // Setup
    void add_symbol(symbol_node* new_symbol);
    std::string error_message;
    std::string filename;
    ERRORS errors;
    // Navigation
    std::vector<symbol_node*> scopes; // Vector of head pointers to different scopes
};


#endif //CPLANE_SYMBOL_TABLE_H
