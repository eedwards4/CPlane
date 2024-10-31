//
// Created by Ethan Edwards on 10/24/2024.
//

#include "symbol_table.h"

// Destructor
symbol_table::~symbol_table() {
    for (symbol_node* current : scopes){ // 1. Iterate through each scope
        while (current != nullptr) { // 2. Iterate through each symbol in the current scope
            symbol_node* temp = current;
            current = current->getNext();
            delete temp; // 3. Delete the current symbol node
        }
    }
}


