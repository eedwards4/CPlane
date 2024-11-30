//
// Created by Logan Puntous on 11/8/2024.
//

#include "errors.h"
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
void ERRORS::ENC_ERROR(std::string filename, int code, int line, int column, std::string error_msg){
    // New error
    ERROR terror;
    // There are errors
    exists = true;
    // Setting
    terror.filename = filename;
    terror.code = code;
    terror.print_statement = error_msg;
    terror.line = line;
    terror.column = column;

    // Checking for dupe errors
    for ( int e = 0; e < VERRORS.size(); e++ ){
        if ( terror.print_statement == VERRORS[e].print_statement && terror.line == VERRORS[e].line && terror.column == VERRORS[e].column ){
            return;
        }
    }

    // Adding the scary to the list of terries
    VERRORS.push_back(terror);
}


// Goes through all errors and prints them. Then exits.
void ERRORS::STOP_SYNTAX_ERRORS(){
    // No errors continue
    if ( exists == false) {
        return;
    }
    // Errors exist so print them johns
    std::cerr << std::endl << "Compilation failed due to following error(s)" << std::endl;
    ERROR terror; // Temp error
    // Loop through errors and print them johns

    for ( int e = 0; e < VERRORS.size(); e++ ){
        terror = VERRORS[e]; 
        printHelper(terror);
        if ( terror.line > 9 ){
            std::cerr << "   " << terror.line << " |" << std::endl << "      |" << std::endl; // Formatting
        } else {
            std::cerr << "    " << terror.line << " |" << std::endl << "      |" << std::endl;
        }
       
    }
    
    // Exiting with code
    exit(VERRORS[VERRORS.size()-1].code); 
}


// Only prints final error. :(
void ERRORS::RDP_SYNTAX_ERRORS(){
    // No errors continue
    if ( exists == false) {
        return;
    }
    // Errors exist so print that john
    std::cerr << std::endl << "Compilation failed due to following error(s)" << std::endl;
    ERROR terror = VERRORS[VERRORS.size()-1];
    printHelper(terror);
    if ( terror.line > 9 ){
        std::cerr << "   " << terror.line << " |" << std::endl << "      |" << std::endl; // Formatting
    } else if ( terror.line > 99 ){
        std::cerr << "  " << terror.line << " |" << std::endl << "      |" << std::endl;
    } else if ( terror.line > 999 ){
        std::cerr << " " << terror.line << " |" << std::endl << "      |" << std::endl;
    } else {
        std::cerr << "    " << terror.line << " |" << std::endl << "      |" << std::endl;
    }

    // Exiting with code
    exit(terror.code);
}









