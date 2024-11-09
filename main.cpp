//
// Created by Ethan Edwards on 8/20/24.
//

#include <iostream>
#include <string>
#include <vector>

#include "errors.h"
#include "ast.h"
#include "tokenizer.h"
#include "exec_path.h"
#include "symbol_table.h"

using namespace std;

// Precompile options
#define SKIP_ARGS 0 // (1 = default input file)

int main(int argc, char* argv[]) {
    string filename;
    if (SKIP_ARGS || argc == 1){
        filename = "tests/programming_assignment_5-test_file_1.c";
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
    
    // Generate output filename base
    string output_base = "./output_files/" + filename.substr(6, filename.size() - 2);
    cout << "Input file: " << filename << endl;
    // Setting up tokenizer
    exec_path path;
    tokenizer t(&path);
    // Tokenize the file
    cout << "Tokenizing file --> ";
    t.tokenize(filename);
    // Error handling
    cout << "Checking for syntax errors --> ";
    errors e;
    e.check_syntax(&path);
    // Remove newlines from the path as they are now unnecessary
    path.remove_newlines();
    // Generate symbol table
    cout << "Building symbol table --> ";
    symbol_table sym_table;
    sym_table.build_table(path);
    // Generate ast
    cout << "Building AST --> ";
    ast as_tree;
    as_tree.build_tree(path.get_head(), sym_table);

    cout << "Generating output files... " << endl;
    // Outputting the tokens
    string tokens_filename = output_base + "_tokens.txt";
    path.print_tokens_to_file(tokens_filename);
    // Outputting the visual path
    string vpath_filename = output_base + "_pathed.txt";
    path.print_visual_path(vpath_filename);
    // Outputting the symbol table
    string symbol_table_filename = output_base + "_symbols.txt";
    sym_table.print_symbols(symbol_table_filename);
    // Outputting the ast
    string ast_filename = output_base + "_ast.txt";
    as_tree.print_tree(ast_filename);

    // Cleanup
    cout << "Done!" << endl << endl;
    cout << "Output files can be found at: " << endl;
    cout << "------------------------------------------------------------------------" << std::endl;
    cout << "Tokens: " << tokens_filename << endl;
    cout << "Visual Path: " << vpath_filename << endl;
    cout << "Symbol Table: " << symbol_table_filename << endl;
    cout << "AST: " << ast_filename << endl;
    cout << "------------------------------------------------------------------------" << std::endl;
    return 0;
}
