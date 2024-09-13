//
// Created by Ethan Edwards on 8/20/24.
//

#include <iostream>
#include <string>
#include <vector>

#include "tokens.h"
#include "errors.h"
#include "tokenizer.h"

using namespace std;

// Precompile options
#define SKIP_ARGS 0

int main(int argc, char* argv[]) {
    string filename;
    if (SKIP_ARGS){
        filename = "test.c";
    } else{
        if (argc < 2){
            cout << "Please provide a file to tokenize." << endl;
            return 1;
        }
        filename = argv[1];
    }

    tokenizer t;
    errors e;
    vector<token> tokens;
    // Execution
    tokens = t.tokenize(filename);
    e.check_syntax(tokens);

    return 0;
}
