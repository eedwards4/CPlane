//
// Created by Ethan Edwards on 10/7/2024.
//

#ifndef CPLANE_EXEC_PATH_H
#define CPLANE_EXEC_PATH_H

#include "tokens.h"
//#include "errors_new.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class exec_node{
public:
    // Management
    exec_node (int type, std::string value, int line, int col, exec_node* next, exec_node* fold){
        this->type = type;
        this->value = value;
        this->next = next;
        this->fold = fold;
        this->line = line;
        this->column = col;
    }
    ~exec_node() = default;
    // Values
    int get_type() const{ return type; }
    std::string get_value() const{ return value; }
    void set_next(exec_node* new_next){ this->next = new_next; }
    void set_fold(exec_node* new_fold){ this->fold = new_fold; }
    // Errors
    int get_line(){ return line; }
    int get_column(){ return column; }
    // std::string get_filename(){ return filename; } // Unused for now, placeholder for multi-file support
    // Navigation
    exec_node* get_next(){ return next; }
    exec_node* get_fold(){ return fold; }

private:
    // Values
    int type;
    std::string value;
    // Errors
    int line;
    int column;
    // std::string filename; // Unused for now, placeholder for multi-file support
    // Navigation
    exec_node* next;
    exec_node* fold;

};

class exec_path {
public:
    // Management
    exec_path(){
        head = nullptr;
        tail = nullptr;
        current = nullptr;
    }
    //m~exec_path();

    void add_node(int type, int line, int col, std::string value="");

    void remove_newlines();

    // Getters
    exec_node* get_head(){ return head; }
    exec_node* get_tail(){ return tail; }

    // Output
    void print_tokens_to_file(std::string filename);

    void print_visual_path(std::string filename);

private:
    // Head/Tail
    exec_node* head;
    exec_node* tail;

    // For building the path
    exec_node* current;

};


#endif //CPLANE_EXEC_PATH_H
