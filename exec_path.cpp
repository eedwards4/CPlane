//
// Created by Ethan Edwards on 9/10/24.
// CURRENTLY UNUSED UNTIL POST-ASSIGNMENT ONE (TOO CLOSE TO DEADLINE TO SAFELY REFACTOR)
//

#include "exec_path.h"

void exec_path::add_node(int type, std::string value) {
    auto* tmp = new exec_node(type, value, nullptr, nullptr);
    if (head == nullptr){
        head = tmp;
        current = tmp;
    }
    else{
        int last_type = current->get_type();
        if (last_type == new_token::OPEN_BRACE || last_type == new_token::OPEN_BRACKET || last_type == new_token::OPEN_PAREN){
            structure.push_back(current);
            current->set_fold(tmp);
            current = tmp;
        }
        else if (last_type == new_token::CLOSE_BRACE || last_type == new_token::CLOSE_BRACKET || last_type == new_token::CLOSE_PAREN){
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
    tail = current;
}
