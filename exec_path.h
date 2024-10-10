//
// Created by Ethan Edwards on 10/7/2024.
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
    void set_next(exec_node* new_next){
        this->next = new_next;
    }
    void set_fold(exec_node* new_fold){
        this->fold = new_fold;
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
    ~exec_path();

    exec_node* get_head(){ return head; }
    exec_node* get_tail(){ return tail; }

    void add_node(int type, std::string value="");

    void print_tokens_to_file(std::string filename);

    void print_visual_path(std::ofstream& out);

    void remove_newlines();

private:
    // Head/Tail
    exec_node* head;
    exec_node* tail;

    // For building the path
    exec_node* current;

};


#endif //CPLANE_EXEC_PATH_H
