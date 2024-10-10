//
// Created by Ethan Edwards on 10/7/2024.
//

#include "exec_path.h"

// Destructor
exec_path::~exec_path() {
    exec_node* current = head;

    while (current != nullptr) {
        exec_node* next;
        if (current->get_next() == nullptr){
            if (current->get_fold() != nullptr){
                next = current->get_fold();
            }
            else{ // Reached the end of the path
                break;
            }
        }
        else{
            next = current->get_next();
        }
        delete current;
        current = next;
    }
}

// Add a node to the path
void exec_path::add_node(int type, std::string value) {
    auto* node = new exec_node(type, value, nullptr, nullptr);

    if (head == nullptr){
        head = node;
        tail = current = node;
    }
    else{
        int last_type = tail->get_type();
        if (last_type == tokens::OPEN_BRACE || last_type == tokens::CLOSE_BRACE){
            current->set_fold(node);
        }
        else{
            current->set_next(node);
        }
        tail = current = node;
    }
}

void exec_path::print_visual_path(){
    exec_node* tmp_head = head;
    exec_node* prev = nullptr;
    int num_spaces_over = 0; // Store the number of spaces to add to the next line when we encounter a fold

    while (tmp_head != nullptr){
        if ((prev != nullptr) && (prev->get_type() == tokens::OPEN_BRACE || prev->get_type() == tokens::CLOSE_BRACE)){
            // Output the visual pointers and an extra set of spaces to start off the new line
            std::cout << " --> NULL" << std::endl;
            std::cout << std::string(num_spaces_over, ' ') << " | \n" <<
                         std::string(num_spaces_over, ' ') << " v \n"; // Down arrow
            std::cout << std::string(num_spaces_over, ' ');
        }
        else if (tmp_head->get_type() >= 9000){ // Custom token
            if (tmp_head == this->head || ((prev != nullptr) && (prev->get_type() == tokens::OPEN_BRACE || prev->get_type() == tokens::CLOSE_BRACE))){
                std::cout << tmp_head->get_value();
            } else{
                std::cout << " --> " << tmp_head->get_value();
                num_spaces_over += 5;
            }
            num_spaces_over += tmp_head->get_value().size();
        }
        else{
            std::cout << " --> " << char(tmp_head->get_type());
            num_spaces_over += 6;
        }
        prev = tmp_head;
        if (tmp_head->get_type() == tokens::OPEN_BRACE || tmp_head->get_type() == tokens::CLOSE_BRACE){
            tmp_head = tmp_head->get_fold();
        } else{
            tmp_head = tmp_head->get_next();
        }
    }
}

void exec_path::remove_newlines() {
    exec_node* tmp_head = head;
    exec_node* prev = nullptr;

    while (tmp_head != nullptr){
        bool leading_newline = false;

        if (tmp_head->get_type() == tokens::NEWLINE){
            if (prev == nullptr){ // Handle leading newline by moving head
                this->head = tmp_head->get_next();
                exec_node* del_obj = tmp_head;
                tmp_head = tmp_head->get_next();
                delete del_obj;
                leading_newline = true;
            }
            else if (prev->get_type() == tokens::OPEN_BRACE || prev->get_type() == tokens::CLOSE_BRACE){ // Handle newline after a fold
                prev->set_fold(tmp_head->get_next());
                exec_node* del_obj = tmp_head;
                tmp_head = tmp_head->get_next();
                delete del_obj;
            }
            else{ // Remove the newline
                prev->set_next(tmp_head->get_next());
                exec_node* del_obj = tmp_head;
                tmp_head = tmp_head->get_next();
                delete del_obj;
            }
        } else {
            prev = tmp_head;
        }

        if (!leading_newline && tmp_head != nullptr){ // Check that tmp_head is not null
            if (tmp_head->get_next() != nullptr && (tmp_head->get_next()->get_type() == tokens::OPEN_BRACE || tmp_head->get_next()->get_type() == tokens::CLOSE_BRACE)) {
                tmp_head = tmp_head->get_fold();
            } else {
                tmp_head = tmp_head->get_next();
            }
        }
    }
}
