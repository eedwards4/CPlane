//
// Created by Ethan Edwards on 9/10/24.
// CURRENTLY UNUSED UNTIL POST-ASSIGNMENT ONE (TOO CLOSE TO DEADLINE TO SAFELY REFACTOR)
//

#include "exec_path.h"

void exec_path::add_node(token t) {
    auto* tmp = new exec_node(t, nullptr, nullptr);
    if (head == nullptr){
        head = tmp;
        current = tmp;
    }
    else if (current->get_token().get_type() == 0){
        char prevchar = current->get_token().get_char_value();
        if (prevchar == '{' || prevchar == '[' || prevchar == '('){
            structure.push_back(current);
            current->set_fold(tmp);
            current = tmp;
        }
        else if (prevchar == '}' || prevchar == ']' || prevchar == ')'){
            current = structure.back(); // Return to outside of fold
            structure.pop_back();
            current->set_next(tmp);
            current = tmp;
        }
        else{
            current->set_next(tmp);
            current = tmp;
        }
    }
    else if (current->get_token().get_type() == 1 || current->get_token().get_type() == 2){
        current->set_next(tmp);
        current = tmp;
    }
    tail = current;
}