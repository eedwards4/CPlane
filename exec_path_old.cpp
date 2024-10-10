//
// Created by Ethan Edwards on 9/10/24.
//

#include "exec_path_old.h"

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

void exec_path::print_tokens_to_file(std::string filename) {

    std::string out_filename = "./output_files/" + filename.substr(6, filename.size() - 2) + "_tokenized.txt";
    //std::string out_filename_windows = ".\\output_files\\" + filename.substr(6, filename.size() - 2) + "_tokenized.txt";
    
    // Trying mac directory
    std::ofstream out(out_filename);
    if (!out) {
        out.close();
	std::cerr << "Error opening output file: " << out_filename << std::endl;
        exit(1);
    }
    std::cout << "Generated file: " + out_filename << std::endl;

    
    exec_node* temp = head;
    out << "\nToken list:\n\n";
    std::vector<exec_node*> printstructure;
    while (temp != nullptr){
        bool printing = false;
        if (temp->get_type() != tokens::NEWLINE){
            out << "Token type: ";
            printing = true;
        }
        bool folding_in = false;
        bool folding_out = false;
        switch(temp->get_type()){
            case tokens::TOKEN_AS_STRING:
                out << "IDENTIFIER\n" << "Token:      " << temp->get_value() << std::endl;
                break;

            case tokens::STRING_LITERAL:
                out << "STRING\n" << "Token:      " << temp->get_value() << std::endl;
                break;

            case tokens::CHAR_LITERAL:
                out << "STRING\n" << "Token:      " << temp->get_value() << std::endl;
                break;

            case tokens::INT_AS_STRING:
                out << "INTEGER\n" << "Token:      " << temp->get_value() << std::endl;
                break;

            case tokens::FLOAT_AS_STRING:
                out << "FLOAT\n" << "Token:      " << temp->get_value() << std::endl;
                break;

            case tokens::NEWLINE:
                break;

            case tokens::OPEN_BRACE:
                out << "L_BRACE\n" << "Token:      {" << std::endl;
                folding_in = true;
                printstructure.push_back(temp);
                break;

            case tokens::CLOSE_BRACE:
                out << "R_BRACE\n" << "Token:      }" << std::endl;
                folding_out = true;
                break;

            case tokens::OPEN_BRACKET:
                out << "L_BRACKET\n" << "Token:      [" << std::endl;
                folding_in = true;
                printstructure.push_back(temp);
                break;

            case tokens::CLOSE_BRACKET:
                out << "R_BRACKET\n" << "Token:      ]" << std::endl;
                folding_out = true;
                break;

            case tokens::OPEN_PAREN:
                out << "L_PAREN\n" << "Token:      (" << std::endl;
                folding_in = true;
                printstructure.push_back(temp);
                break;

            case tokens::CLOSE_PAREN:
                out << "R_PAREN\n" << "Token:      )" << std::endl;
                folding_out = true;
                break;

            case tokens::PLUS_PLUS:
                out << "PLUS_PLUS\n" << "Token:      ++" << std::endl;
                break;

            case tokens::PLUS_EQUALS:
                out << "PLUS_EQUALS\n" << "Token:      +=" << std::endl;
                break;

            case tokens::MINUS_MINUS:
                out << "MINUS_MINUS\n" << "Token:      --" << std::endl;
                break;

            case tokens::MINUS_EQUALS:
                out << "MINUS_EQUALS\n" << "Token:      -=" << std::endl;
                break;

            case tokens::RIGHT_SLIM_ARROW:
                out << "RIGHT_SLIM_ARROW\n" << "Token:      ->" << std::endl;
                break;

            case tokens::BOOLEAN_AND:
                out << "BOOLEAN_AND\n" << "Token:      &&" << std::endl;
                break;

            case tokens::AND_EQUALS:
                out << "AND_EQUALS\n" << "Token:      &=" << std::endl;
                break;

            case tokens::BOOLEAN_OR:
                out << "BOOLEAN_OR\n" << "Token:      ||" << std::endl;
                break;

            case tokens::OR_EQUALS:
                out << "OR_EQUALS\n" << "Token:      |=" << std::endl;
                break;

            case tokens::NOT_EQUALS:
                out << "NOT_EQUALS\n" << "Token:      !=" << std::endl;
                break;

            case tokens::MOD_EQUALS:
                out << "MOD_EQUALS\n" << "Token:      %=" << std::endl;
                break;

            case tokens::TIMES_EQUALS:
                out << "TIMES_EQUALS\n" << "Token:      *=" << std::endl;
                break;

            case tokens::EQUALS_EQUALS:
                out << "BOOLEAN_EQUAL\n" << "Token:      ==" << std::endl;
                break;

            case tokens::XOR_EQUALS:
                out << "XOR_EQUALS\n" << "Token:      ^=" << std::endl;
                break;

            case tokens::TOKEN_PASTE:
                out << "TOKEN_PASTE\n" << "Token:      ##" << std::endl;
                break;

            case tokens::GREATER_EQUALS:
                out << "GT_EQUAL\n" << "Token:      >=" << std::endl;
                break;

            case tokens::RIGHT_SHIFT_EQUALS:
                out << "RIGHT_SHIFT_EQUALS\n" << "Token:      >>=" << std::endl;
                break;

            case tokens::RIGHT_SHIFT:
                out << "RIGHT_SHIFT\n" << "Token:     >>" << std::endl;
                break;

            case tokens::LESS_EQUALS:
                out << "LT_EQUAL\n" << "Token:      <=" << std::endl;
                break;

            case tokens::LEFT_SHIFT_EQUALS:
                out << "LEFT_SHIFT_EQUALS\n" << "Token:      <<=" << std::endl;
                break;

            case tokens::LEFT_SHIFT:
                out << "LEFT_SHIFT\n" << "Token:      <<" << std::endl;
                break;

            case tokens::DIVIDE_EQUALS:
                out << "DIVIDE_EQUALS\n" << "Token:      /=" << std::endl;
                break;

            case tokens::ELLIPSIS:
                out << "ELLIPSIS\n" << "Token:      ..." << std::endl;
                break;

            case tokens::SCOPE:
                out << "SCOPE\n" << "Token:      ::" << std::endl;
                break;

            case tokens::LITERAL_DEF:
                out << "LITERAL_DEF\n" << "Token:      L\"\" or L''" << std::endl;
                break;

            // Single-character operators
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
        if (folding_in){
            temp = temp->get_fold();
        } else if (folding_out){
            temp = printstructure.back()->get_next();
            printstructure.pop_back();
        } else {
            temp = temp->get_next();
        }
        if (printing){
            out << std::endl;
        }
    }
    out << std::endl;
    out.close();

    std::cout << "Finished writing to file." << std::endl;
}

void exec_path::remove_newlines() {
	std::vector<exec_node*> foldStack;
	exec_node* temp;
	
	while (head->get_type() == tokens::NEWLINE) {
		temp = head->get_next();
		delete head;
		head = temp;
	}
	exec_node* ptr = head;

	while (ptr != tail && ptr != nullptr) {
		if (ptr->get_next() != nullptr && ptr->get_next()->get_type() == tokens::NEWLINE) {
			temp = ptr->get_next();
			while (temp != nullptr && temp->get_type() == tokens::NEWLINE) {
				temp = temp->get_next();
			}
			ptr->set_next(temp);
		}
		if (ptr->get_fold() != nullptr && ptr->get_fold()->get_type() == tokens::NEWLINE) {
			temp = ptr->get_fold();
			while (temp != nullptr && temp->get_type() == tokens::NEWLINE) {
				temp = temp->get_next();
			}
			ptr->set_fold(temp);
		}

		if (ptr->get_fold() != nullptr) {
			foldStack.push_back(ptr);
			ptr = ptr->get_fold();
		} else if (ptr->get_next() == nullptr && !foldStack.empty()) {
			ptr = foldStack.back()->get_next();
			foldStack.pop_back();
		} else {
			ptr = ptr->get_next();
		}
	}
}