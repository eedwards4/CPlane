#include <iostream>
#include <fstream>
#include <sstream>

#include "remove_comments.h"
#include "token.h"
#include "tokenizer.h"
#include "recursive_descent_parser.h"
#include "symbol_table.h"

bool print_tokens(Tokenizer&, std::string);

int main(int argc, char* argv[]) {
    if (argc < 3) { std::cerr << "ERROR: Please specify input and output file in arguments\n"; return 1; }
    if (argc > 3) { std::cerr << "ERROR: Too mant arguments\n"; return 1; }

    removeComments(argv[1], argv[2]);
	
	Tokenizer tokenizer = Tokenizer(argv[2]);
	// print_tokens(tokenizer, argv[2]);

	RecursiveDescentParser rdp = RecursiveDescentParser(tokenizer, argv[2]);
	if (!rdp.checkSyntax()) { return 1; }
	// rdp.printCST();

	SymbolTable symbol_table = SymbolTable(rdp.head(), argv[2]);
	if (!symbol_table.create()) { return 1; }
	symbol_table.print();
}


bool print_tokens(Tokenizer& tokenizer, std::string outfile) {
    std::ofstream out(outfile, std::ios::out);

    out << "\nToken List:\n\n";

    Token* token = tokenizer.getToken();
    while (!token->eof()) {
        out << "Token type: " << token->type() << std::endl;
        out << "Token:      " << token->value() << std::endl << std::endl;
        token = tokenizer.getToken();
    }

    out.close();
    return true;
}