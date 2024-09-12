//
// Created by Ethan Edwards on 9/10/24.
// CURRENTLY UNUSED UNTIL POST-ASSIGNMENT ONE (TOO CLOSE TO DEADLINE TO SAFELY REFACTOR)
//

#ifndef CPLANE_EXEC_PATH_H
#define CPLANE_EXEC_PATH_H

#include "tokens.h"

#include <string>
#include <vector>
#include <iostream>

class exec_node{
public:
    exec_node() = default;
    ~exec_node() = default;

    exec_node (token t, exec_node* next, exec_node* fold){
        this->t = t;
        this->next = next;
        this->fold = fold;
    }

    token get_token(){
        return t;
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
    token t;
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

    void add_node(token t);

    void print_path(){
        exec_node* temp = head;
        std::vector<exec_node> printstructure;
        while (temp != nullptr){
            if (temp->get_token().get_type() == 0){
                std::cout << temp->get_token().get_char_value();
                if (temp->get_token().get_char_value() == '{' || temp->get_token().get_char_value() == '[' || temp->get_token().get_char_value() == '('){
                    printstructure.push_back(*temp);
                    temp = temp->get_fold();
                } else if (temp->get_token().get_char_value() == '}' || temp->get_token().get_char_value() == ']' || temp->get_token().get_char_value() == ')'){
                    temp = printstructure.back().get_next();
                    printstructure.pop_back();
                }
                else{
                    temp = temp->get_next();
                }
            }
            else if (temp->get_token().get_type() == 1){
                std::cout << temp->get_token().get_str_value();
                temp = temp->get_next();
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
