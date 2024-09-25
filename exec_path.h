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
#include <fstream>

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

    exec_node* get_head(){ return head; }
    exec_node* get_tail(){ return tail; }

    void add_node(int type, std::string value = "");

    void print_path();

    void print_tokens();

    void print_tokens_to_file();

private:
    // Abs. head/tail
    exec_node* head;
    exec_node* tail;
    // Navigation
    exec_node* current;
    std::vector<exec_node*> structure;
};


#endif //CPLANE_EXEC_PATH_H
