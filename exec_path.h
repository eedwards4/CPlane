//
// Created by Ethan Edwards on 9/10/24.
// CURRENTLY UNUSED UNTIL POST-ASSIGNMENT ONE (TOO CLOSE TO DEADLINE TO SAFELY REFACTOR)
//

#ifndef CPLANE_EXEC_PATH_H
#define CPLANE_EXEC_PATH_H

#include "new_token.h"

#include <string>
#include <vector>
#include <iostream>

class exec_node{
public:
    exec_node() = default;
    ~exec_node() = default;

    exec_node (int type, std::string value, exec_node* next, exec_node* fold){
        this->type = type;
        this->value = value;
        this->next = next;
        this->fold = fold;
    }

    int get_type() const{
        return type;
    }
    std::string get_value() const{
        return value;
    }
    exec_node* get_next(){
        return next;
    }
    exec_node* get_fold(){
        return fold;
    }
    void set_next(exec_node* next){
        this->next = next;
    }
    void set_fold(exec_node* fold){
        this->fold = fold;
    }

private:
    int type;
    std::string value;
    exec_node* next;
    exec_node* fold;

};

class exec_path {
public:
    exec_path(){
        head = nullptr;
        tail = nullptr;
        current = nullptr;
    }
    ~exec_path(){
        exec_node* temp = head;
        while (temp != nullptr){
            exec_node* next = temp->get_next();
            delete temp;
            temp = next;
        }
    }

    void add_node(int type, std::string value = "");

    void print_path(){
        exec_node* temp = head;
        std::vector<exec_node*> printstructure;
        while (temp != nullptr){
            if (temp->get_type() == new_token::TOKEN_AS_STRING || temp->get_type() == new_token::STRING_LITERAL || temp->get_type() == new_token::CHAR_LITERAL ||
                temp->get_type() == new_token::INT_AS_STRING || temp->get_type() == new_token::FLOAT_AS_STRING){
                std::cout << temp->get_value();
                temp = temp->get_next();
            }
            else{
                if (temp->get_type() == new_token::NEWLINE){
                    std::cout << std::endl;
                    temp = temp->get_next();
                } else{
                    if (temp->get_type() == new_token::OPEN_BRACE || temp->get_type() == new_token::OPEN_BRACKET || temp->get_type() == new_token::OPEN_PAREN){
                        if (temp->get_type() == new_token::OPEN_BRACE){
                            std::cout << "{";
                        } else if (temp->get_type() == new_token::OPEN_BRACKET){
                            std::cout << "[";
                        } else{
                            std::cout << "(";
                        }
                        printstructure.push_back(temp);
                        temp = temp->get_fold();
                    } else if (temp->get_type() == new_token::CLOSE_BRACE || temp->get_type() == new_token::CLOSE_BRACKET || temp->get_type() == new_token::CLOSE_PAREN){
                        if (temp->get_type() == new_token::CLOSE_BRACE){
                            std::cout << "}";
                        } else if (temp->get_type() == new_token::CLOSE_BRACKET){
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

private:
    // Abs. head/tail
    exec_node* head;
    exec_node* tail;
    // Navigation
    exec_node* current;
    std::vector<exec_node*> structure;
};


#endif //CPLANE_EXEC_PATH_H
