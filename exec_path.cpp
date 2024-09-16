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
        if (last_type == tokens::OPEN_BRACE || last_type == tokens::OPEN_BRACKET || last_type == tokens::OPEN_PAREN){
            structure.push_back(current);
            current->set_fold(tmp);
            current = tmp;
        }
        else if (last_type == tokens::CLOSE_BRACE || last_type == tokens::CLOSE_BRACKET || last_type == tokens::CLOSE_PAREN){
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

void exec_path::print_path() {
    exec_node* temp = head;
    std::vector<exec_node*> printstructure;
    while (temp != nullptr){
        if (temp->get_type() == tokens::TOKEN_AS_STRING || temp->get_type() == tokens::STRING_LITERAL || temp->get_type() == tokens::CHAR_LITERAL ||
            temp->get_type() == tokens::INT_AS_STRING || temp->get_type() == tokens::FLOAT_AS_STRING){
            std::cout << temp->get_value();
            temp = temp->get_next();
        }
        else{
            if (temp->get_type() == tokens::NEWLINE){
                std::cout << std::endl;
                temp = temp->get_next();
            } else{
                if (temp->get_type() == tokens::OPEN_BRACE || temp->get_type() == tokens::OPEN_BRACKET || temp->get_type() == tokens::OPEN_PAREN){
                    if (temp->get_type() == tokens::OPEN_BRACE){
                        std::cout << "{";
                    } else if (temp->get_type() == tokens::OPEN_BRACKET){
                        std::cout << "[";
                    } else{
                        std::cout << "(";
                    }
                    printstructure.push_back(temp);
                    temp = temp->get_fold();
                } else if (temp->get_type() == tokens::CLOSE_BRACE || temp->get_type() == tokens::CLOSE_BRACKET || temp->get_type() == tokens::CLOSE_PAREN){
                    if (temp->get_type() == tokens::CLOSE_BRACE){
                        std::cout << "}";
                    } else if (temp->get_type() == tokens::CLOSE_BRACKET){
                        std::cout << "]";
                    } else{
                        std::cout << ")";
                    }
                    temp = printstructure.back()->get_next();
                    printstructure.pop_back();
                } else{
                    std::cout << char(temp->get_type());
                    temp = temp->get_next();
                }
            }
        }
    }
}
