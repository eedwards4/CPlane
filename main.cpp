//
// Created by Ethan Edwards on 8/20/24.
//

#include <iostream>
#include <string>
#include <vector>

#include "errors.h"
#include "tokenizer.h"

using namespace std;

// Precompile options
#define SKIP_ARGS 0

int main(int argc, char* argv[]) {
    string filename;
    if (SKIP_ARGS){
        filename = "C:\\Users\\evane\\Documents\\CPPprojects\\CPlane\\tests\\programming_assignment_1-test_file_1.c";
    } else{
        if (argc < 2){
            cout << "Please provide a file to tokenize." << endl;
            return 1;
        }
        filename = argv[1];
    }

    exec_path path;
    tokenizer t(&path);
    t.tokenize(filename);
    path.print_path();
    errors e;
    e.check_syntax(&path);
    return 0;
}
