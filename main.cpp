//
// Created by Ethan Edwards on 8/20/24.
//

#include <iostream>
#include <string>
#include <vector>
#include <cstring>


#include "errors.h"
#include "ast.h"
#include "tokenizer.h"
#include "exec_path.h"
#include "recursive_descent_parser.h"
#include "symbol_table.h"
#include "interpreter.h"

using namespace std;


int main(int argc, char* argv[]) {
    string filename;
    bool debug_mode = false;
    int exit_code = 0;
    
    /* --------------- USE FILE HERE IF USING IDE ----------------*/
    if ( argc == 1 ){
        filename = "tests/programming_assignment_6-test_file_1.c";
        debug_mode = false; // If u want debug mode on or off
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

    // Setting up the errors class.
    ERRORS errors;
    
    // Generate output filename base
    string output_base = "./output_files/" + filename.substr(6, filename.size() - 2);
    if ( debug_mode ){ cout << "Input file: " << filename << endl; }
    // Setting up tokenizer
   
    exec_path path;
    tokenizer t(&path);
    // Tokenize the file
    if ( debug_mode ){ cout << "Tokenizing file --> "; }
    t.tokenize(filename, errors);
    if ( debug_mode ){ std::cout << "NOW CHECKING TOKENIZED ERRORS..." << std::endl;}
    errors.STOP_SYNTAX_ERRORS(); // CHECKING ERRORS BEFORE CONTINUING
    // Error handling
    if ( debug_mode ){ cout << "Checking for syntax errors --> "; }

    // Recursive descent parser
	RecursiveDescentParser rdp(filename, errors);
    if ( debug_mode ){ std::cout << "NOW CHECKING RDP ERRORS..." << std::endl;}
	if(!rdp.check_syntax(path.get_head())) {}
    // TODO THIS IS ERRORING A BUNCH SO NEED TO CATCH WHY
    rdp.returnErrors();

    // Remove newlines from the path as they are now unnecessary
    path.remove_newlines();

    if ( debug_mode ){ cout << "Building symbol table --> "; }

    // Generate symbol table
    symbol_table sym_table(errors, filename);
    sym_table.build_table(path);
    if ( debug_mode ){ std::cout << "NOW CHECKING SYMBOL ERRORS..." << std::endl; }
    errors = sym_table.returnErrors();
    errors.STOP_SYNTAX_ERRORS(); // CHECKING ERRORS BEFORE CONTINUING
    
    if ( debug_mode ){ cout << "Building AST --> "; }

     // Generate ast
    ast as_tree;
    //SEGMENTATION FAULT HERE for ass2 file 6 cause rdp or symtable not catching 
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
        cout << endl;
        cout << "Output files: " << endl;
        cout << "Tokens: " << tokens_filename << endl;
        cout << "Visual Path: " << vpath_filename << endl;
        cout << "Symbol Table: " << symbol_table_filename << endl;
        cout << "AST: " << ast_filename << endl;
        cout << "------------------------------------------------------------------------" << std::endl;
    }
    

    // Interpreter time bay bee!
    Interpreter r(as_tree, sym_table, errors);
    r.begin(debug_mode);
    
    // We should not get here if program executed fully
    return 1;
}
