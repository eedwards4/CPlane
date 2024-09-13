//
// Created by Ethan Edwards on 8/20/24.
//

#include <iostream>
#include <string>
#include <vector>

#include "tokens.h"
#include "errors.h"
#include "tokenizer.h"
#include "tokenizer_new.h"

using namespace std;

// Precompile options
#define SKIP_ARGS 1

int main(int argc, char* argv[]) {
    string filename;
    if (SKIP_ARGS){
        filename = "C:\\Users\\evane\\Documents\\CPPprojects\\CPlane\\tests\\programming_assignment_1-test_file_4.c";
    } else{
        if (argc < 2){
            cout << "Please provide a file to tokenize." << endl;
            return 1;
        }
        filename = argv[1];
    }
/*
    tokenizer t;
    errors e;
    vector<token> tokens;
    // Execution
    tokens = t.tokenize(filename);
    e.check_syntax(tokens);
*/
    exec_path path;
    tokenizer_new t(&path);
    t.tokenize(filename);
    path.print_path();
    return 0;
}