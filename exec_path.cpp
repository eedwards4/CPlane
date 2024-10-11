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

    if (head == nullptr) {
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

// Print tokens to file
void exec_path::print_tokens_to_file(std::string filename) {
    std::ofstream out(filename);
    exec_node* tmp_head = head;
    while (tmp_head != nullptr) {
        out << "Token type: ";
        if (tmp_head->get_type() >= 9000) { // Multi-character tokens
            switch (tmp_head->get_type()) {
                // Folding tokens
                case tokens::OPEN_BRACE:
                    out << "L_BRACE\n" << "Token:      {" << std::endl;
                    tmp_head = tmp_head->get_fold();
                    break;

                case tokens::CLOSE_BRACE:
                    out << "R_BRACE\n" << "Token:      }" << std::endl;
                    tmp_head = tmp_head->get_fold();
                    break;

                // Other custom tokens
                case tokens::TOKEN_AS_STRING:
                    out << "IDENTIFIER\n" << "Token:      " << tmp_head->get_value() << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::STRING_LITERAL: case tokens::CHAR_LITERAL:
                    out << "STRING\n" << "Token:      " << tmp_head->get_value() << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::INT_AS_STRING:
                    out << "INT\n" << "Token:      " << tmp_head->get_value() << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::FLOAT_AS_STRING:
                    out << "FLOAT\n" << "Token:      " << tmp_head->get_value() << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::NEWLINE:
                    break;

                case tokens::OPEN_BRACKET:
                    out << "L_BRACKET\n" << "Token:      [" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::CLOSE_BRACKET:
                    out << "R_BRACKET\n" << "Token:      ]" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::OPEN_PAREN:
                    out << "L_PAREN\n" << "Token:      (" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::CLOSE_PAREN:
                    out << "R_PAREN\n" << "Token:      )" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::PLUS_PLUS:
                    out << "PLUS_PLUS\n" << "Token:      ++" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::MINUS_MINUS:
                    out << "MINUS_MINUS\n" << "Token:      --" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::PLUS_EQUALS:
                    out << "PLUS_EQUALS\n" << "Token:      +=" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::MINUS_EQUALS:
                    out << "MINUS_EQUALS\n" << "Token:      -=" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::TIMES_EQUALS:
                    out << "TIMES_EQUALS\n" << "Token:      *=" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::DIVIDE_EQUALS:
                    out << "DIVIDE_EQUALS\n" << "Token:      /=" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::MOD_EQUALS:
                    out << "MOD_EQUALS\n" << "Token:      %=" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::EQUALS_EQUALS:
                    out << "EQUALS_EQUALS\n" << "Token:      ==" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::NOT_EQUALS:
                    out << "NOT_EQUALS\n" << "Token:      !=" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::LESS_EQUALS:
                    out << "LESS_EQUALS\n" << "Token:      <=" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::GREATER_EQUALS:
                    out << "GREATER_EQUALS\n" << "Token:      >=" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::BOOLEAN_AND:
                    out << "BOOLEAN_AND\n" << "Token:      &&" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::BOOLEAN_OR:
                    out << "BOOLEAN_OR\n" << "Token:      ||" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::RIGHT_SHIFT:
                    out << "RIGHT_SHIFT\n" << "Token:      >>" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::LEFT_SHIFT:
                    out << "LEFT_SHIFT\n" << "Token:      <<" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::RIGHT_SHIFT_EQUALS:
                    out << "RIGHT_SHIFT_EQUALS\n" << "Token:      >>=" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::LEFT_SHIFT_EQUALS:
                    out << "LEFT_SHIFT_EQUALS\n" << "Token:      <<=" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::AND_EQUALS:
                    out << "AND_EQUALS\n" << "Token:      &=" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::OR_EQUALS:
                    out << "OR_EQUALS\n" << "Token:      |=" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::XOR_EQUALS:
                    out << "XOR_EQUALS\n" << "Token:      ^=" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::TOKEN_PASTE:
                    out << "TOKEN_PASTE\n" << "Token:      ##" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::SCOPE:
                    out << "SCOPE\n" << "Token:      ::" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::ELLIPSIS:
                    out << "ELLIPSIS\n" << "Token:      ..." << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::LITERAL_DEF:
                    out << "LITERAL_DEF\n" << "Token:      L" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                case tokens::RIGHT_SLIM_ARROW:
                    out << "RIGHT_SLIM_ARROW\n" << "Token:      ->" << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;

                default:
                    out << tmp_head->get_value() << std::endl;
                    tmp_head = tmp_head->get_next();
                    break;
            }
        } // Multi-character tokens
        else { // Single character tokens
            switch (tmp_head->get_type()) {
                case '+':
                    out << "PLUS\n" << "Token:      +" << std::endl;
                    break;

                case '-':
                    out << "MINUS\n" << "Token:      -" << std::endl;
                    break;

                case '*':
                    out << "ASTERISK\n" << "Token:      *" << std::endl;
                    break;

                case '/':
                    out << "DIVIDE\n" << "Token:      /" << std::endl;
                    break;

                case '%':
                    out << "MODULO\n" << "Token:      %" << std::endl;
                    break;

                case '=':
                    out << "ASSIGNMENT_OPERATOR\n" << "Token:      =" << std::endl;
                    break;

                case '^':
                    out << "XOR\n" << "Token:      ^" << std::endl;
                    break;

                case '&':
                    out << "AND\n" << "Token:      &" << std::endl;
                    break;

                case '|':
                    out << "OR\n" << "Token:      |" << std::endl;
                    break;

                case '~':
                    out << "NOT\n" << "Token:      ~" << std::endl;
                    break;

                case '!':
                    out << "NOT\n" << "Token:      !" << std::endl;
                    break;

                case '<':
                    out << "LT\n" << "Token:      <" << std::endl;
                    break;

                case '>':
                    out << "GT\n" << "Token:      >" << std::endl;
                    break;

                case '?':
                    out << "TERNARY\n" << "Token:      ?" << std::endl;
                    break;

                case ':':
                    out << "TERNARY\n" << "Token:      :" << std::endl;
                    break;

                case ',':
                    out << "COMMA\n" << "Token:      ," << std::endl;
                    break;

                case ';':
                    out << "SEMICOLON\n" << "Token:      ;" << std::endl;
                    break;

                case '.':
                    out << "DOT\n" << "Token:      ." << std::endl;
                    break;

                case '"':
                    out << "DOUBLE_QUOTE\n" << "Token:      \"" << std::endl;
                    break;

                case '\'':
                    out << "SINGLE_QUOTE\n" << "Token:      '" << std::endl;
                    break;

                default:
                    break;
            }
            tmp_head = tmp_head->get_next();
        } // Single character tokens
        out << std::endl;
    }
    out.close();
}

void exec_path::print_visual_path(std::string filename) {
    exec_node* tmp_head = head;
    exec_node* prev = nullptr;
    int num_spaces_over = 0;  // Store the number of spaces to add to the next line when we encounter a fold

    std::ofstream out(filename);

    while (tmp_head != nullptr) {
        if ((prev != nullptr) && (prev->get_type() == tokens::OPEN_BRACE || prev->get_type() == tokens::CLOSE_BRACE)) {
            // Output the visual pointers and an extra set of spaces to start the new line
            out << " --> NULL" << std::endl;
            out << std::string(num_spaces_over, ' ') << "| \n"
                << std::string(num_spaces_over, ' ') << "v \n";  // Down arrow
            out << std::string(num_spaces_over, ' ') << tmp_head->get_value();
            // Update spaces only for the current token's value length
            num_spaces_over += tmp_head->get_value().size() - 1;
        }
        else if (tmp_head->get_type() >= 9000) {  // Custom token
            if (tmp_head == this->head) {
                out << tmp_head->get_value();
                num_spaces_over += tmp_head->get_value().size() - 1;
            } else {
                out << " --> " << tmp_head->get_value();
                num_spaces_over += 5;  // For " --> "
                num_spaces_over += tmp_head->get_value().size();
            }
        }
        else {
            out << " --> " << char(tmp_head->get_type());
            num_spaces_over += 6;  // " --> " is 5 characters, plus 1 for the char
        }

        prev = tmp_head;

        // Handling folding logic (OPEN_BRACE or CLOSE_BRACE)
        if (tmp_head->get_type() == tokens::OPEN_BRACE || tmp_head->get_type() == tokens::CLOSE_BRACE) {
            if (tmp_head->get_fold() == nullptr) {
                out << " --> NULL" << std::endl;
            }
            tmp_head = tmp_head->get_fold();
        } else {
            tmp_head = tmp_head->get_next();
        }
    }
    out.close();
}

void exec_path::remove_newlines() {
    exec_node* tmp_head = head;
    exec_node* prev = nullptr;

    while (tmp_head != nullptr){
        if (tmp_head->get_type() == tokens::NEWLINE){
            if (prev == nullptr){ // Leading newline
                head = tmp_head->get_next();
                delete tmp_head;
                tmp_head = head;
            }
            else if (prev->get_type() == tokens::OPEN_BRACE || prev->get_type() == tokens::CLOSE_BRACE){
                prev->set_fold(tmp_head->get_next());
                delete tmp_head;
                tmp_head = prev->get_fold();
            }
            else{
                prev->set_next(tmp_head->get_next());
                delete tmp_head;
                tmp_head = prev->get_next();
            }
        } else{
            prev = tmp_head;
            if (tmp_head->get_type() == tokens::OPEN_BRACE || tmp_head->get_type() == tokens::CLOSE_BRACE){
                tmp_head = tmp_head->get_fold();
            } else{
                tmp_head = tmp_head->get_next();
            }
        }
    }
}
