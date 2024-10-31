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

    // Getters


private:

};

class symbol_table {
public:
    // Constructor
    symbol_table() = default;
    ~symbol_table();

    // Setters


private:
    // Setup

    // Navigation
    std::vector<symbol_node*> scopes; // Vector of head pointers to different scopes
};


#endif //CPLANE_SYMBOL_TABLE_H
