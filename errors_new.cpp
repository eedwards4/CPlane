//
// Created by Logan Puntous on 11/8/2024.
//

#include "errors_new.h"
#include <set>

// Default constructors
ERROR::ERROR(){
    //filename ;
    code = 1;
    line = -1;
    column = -1;
    filename = "DEFAULT FILENAME";
    print_statement = "DEFAULT ERROR";
}

ERRORS::ERRORS(){
    exists = false;
}


// Only exists to print the actual statement. Builds message using info
void ERRORS::printHelper(ERROR error){
    // Building message
    std::cerr << error.filename << ":" << error.line << ":" << error.column << ": error: ";
    std::cerr << error.print_statement << std::endl;

}


// Adds an error to the list of errors and continues so the program can get all errors
void ERRORS::ENC_ERROR(std::string filename, int code, int line, int column/*, Symbol symbol??, or string maybe*/){
    // New error
    ERROR terror;
    // There are errors
    exists = true;
    // Setting
    terror.filename = filename;
    terror.line = line;
    terror.column = column;
    terror.code = code;
    // TODO: set correct output message somehow.

    // Adding the scary to the list of errors
    VERRORS.push_back(terror);
}


// Goes through all errors and prints them. Then exits.
void ERRORS::STOP_SYNTAX_ERRORS(){
    if ( exists == false) {
        exit(1);
    }
    std::cerr << "Compilation failed due to following error(s)" << std::endl;
    ERROR terror; // Temp error
    // Loop through errors and print them johns
    for ( int e = 0; e < VERRORS.size(); e++ ){
        terror = VERRORS[e];
        printHelper(terror);
        std::cerr << "|" << std::endl << "|" << std::endl; // Formatting
    }
    
    
    // Exiting
    exit(VERRORS[-1].code); 
}



