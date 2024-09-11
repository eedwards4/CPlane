//
// Created by Ethan Edwards on 8/20/24.
//

#include <iostream>
#include <string>
#include <vector>

#include "tokens.h"
#include "errors.h"
#include "tokenizer.h"
#include "exec_path.h"

using namespace std;

int main(){
    string filename = "C:\\Users\\evane\\Documents\\CPPprojects\\CPlane\\tests\\programming_assignment_1-test_file_2.c";
    tokenizer t;
    errors e;
    vector<token> tokens;
    // Execution
    tokens = t.tokenize(filename);
    for (auto & token : tokens){ // TEMPORARY
        if (token.get_type() == 0){
            cout << "Type: Character, Value: " << token.get_char_value() << endl;
        }
        else if (token.get_type() == 1){
            cout << "Type: String, Value: " << token.get_str_value() << endl;
        }
    }
    e.check_syntax(tokens);
    exec_path pth;
    for (auto & token : tokens){
        pth.add_node(token);
    }
    pth.print_path();

    return 0;
}