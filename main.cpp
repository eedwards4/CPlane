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
        filename = "programming_assignment_2-test_file_6.c";
    } else{
        if (argc != 2){
            cout << "Please provide a single file to tokenize." << endl;
            return 1;
        }
	
	filename = argv[1];
	
	if ( filename.substr(filename.size() - 2) != ".c" ){
	  cout << "Please provide a valid .c file." << endl;
	  return 1;
 	}
    }

    // Setting up tokenizer
    exec_path path;
    tokenizer t(&path);
    // Sending in the input file
    t.tokenize(filename);
    // Generating output file
    path.print_tokens_to_file(filename);
    // Error handling
    errors e;
    e.check_syntax(&path);
    // Cleanup
    return 0;
}
