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
#include "interpreter.h"

using namespace std;


int main(int argc, char* argv[]) {
    string filename;
    bool debug_mode = false;
    int exit_code = 0;
    
    /* --------------- USE FILE HERE IF USING IDE ----------------*/
    if ( argc == 1 ){
        filename = "tests/programming_assignment_5-test_file_1.c";
        debug_mode = true; // If u want debug mode on or off
    }
    /* -----------------------------------------------------------*/
    else if ( argc == 2 ){
        debug_mode = false;
        filename = argv[1];
    }
    else if ( argc == 3 && strcmp(argv[2], "-d") == 0){
        debug_mode = true;
        filename = argv[1];
    } else {
        cout << "Please provide one c input file! (use -d extension for debug mode)" << endl;
        exit(1);
    }
    // Input is good but is the file correct?
    if ( argc != 1){
        filename = argv[1];
    }
    if ( filename.substr(filename.size() - 2) != ".c" ){
        cout << "Please provide a valid .c file." << endl;
	    exit(1);
    }
    if ( debug_mode ){
        cout << "Debug mode ENABLED" << endl;
    }
    
    
    // Generate output filename base
    string output_base = "./output_files/" + filename.substr(6, filename.size() - 2);
    if ( debug_mode ){ cout << "Input file: " << filename << endl; }
    // Setting up tokenizer
    exec_path path;
    tokenizer t(&path);
    // Tokenize the file
    if ( debug_mode ){ cout << "Tokenizing file --> "; }
    t.tokenize(filename);
    // Error handling
    if ( debug_mode ){ cout << "Checking for syntax errors --> "; }
    errors e;
    e.check_syntax(&path);
    // Remove newlines from the path as they are now unnecessary
    path.remove_newlines();
    // Generate symbol table
    if ( debug_mode ){ cout << "Building symbol table --> "; }
    symbol_table sym_table;
    sym_table.build_table(path);
    // Generate ast
    if ( debug_mode ){ cout << "Building AST --> "; }
    ast as_tree;
    as_tree.build_tree(path.get_head(), sym_table);

    if ( debug_mode ){ cout << "Generating output files... " << endl; }
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


    if ( debug_mode ){
        cout << "Output files can be found at: " << endl;
        cout << "------------------------------------------------------------------------" << std::endl;
        cout << "Tokens: " << tokens_filename << endl;
        cout << "Visual Path: " << vpath_filename << endl;
        cout << "Symbol Table: " << symbol_table_filename << endl;
        cout << "AST: " << ast_filename << endl;
        cout << "------------------------------------------------------------------------" << std::endl;
        cout << endl;
    }
    

    // Interpreter time bay bee!
    Interpreter r;
    if ( debug_mode ){
        r.BeginDebug();
    } else {
        r.Begin();
    }
    exit_code = r.Exit();
    std::cout << std::endl;
    return exit_code;
}
