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
#define SKIP_ARGS 1

int main(int argc, char* argv[]) {
    string filename;
    if (SKIP_ARGS){
        filename = "./tests/programming_assignment_2-test_file_6.c";
    } else{
        if (argc != 2){
            cout << "Please provide a single file to tokenize." << endl;
            exit(1);
        }
	
	    filename = argv[1];
	
	    if ( filename.substr(filename.size() - 2) != ".c" ){
	        cout << "Please provide a valid .c file." << endl;
	        exit(1);
 	    }
    }
    cout << "File successfully loaded." << endl;
    // Generate output filename base
    string output_base = "./output_files/" + filename.substr(6, filename.size() - 2);

    // Setting up tokenizer
    exec_path path;
    tokenizer t(&path);
    // Tokenize the file
    cout << "Tokenizing file..." << endl;
    t.tokenize(filename);
    // Error handling
    cout << "Checking for syntax errors..." << endl;
    errors e;
    e.check_syntax(&path);
    // Remove newlines from the path as they are now unnecessary
    path.remove_newlines();

    cout << "Generating output files..." << endl;
    // Outputting the tokens
    string tokens_filename = output_base + "_tokens.txt";
    path.print_tokens_to_file(tokens_filename);
    // Outputting the visual path
    string vpath_filename = output_base + "_pathed.txt";
    path.print_visual_path(vpath_filename);
    // Cleanup
    cout << "Done!" << endl;
    cout << "Output files can be found at: " << endl;
    cout << "Tokens: " << tokens_filename << endl;
    cout << "Visual Path: " << vpath_filename << endl;
    return 0;
}
