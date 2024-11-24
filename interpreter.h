#ifndef CPLANE_INTERPRETER_H
#define CPLANE_INTERPRETER_H

#include "symbol_table.h"
#include "ast.h"
#include "errors_new.h"

#include <stack>
#include <type_traits>
#include <cctype>
#include <cmath>

class Interpreter {
    public:
        Interpreter();
        Interpreter(ast tree, symbol_table& table, ERRORS& errors);

        void begin();
    
    private:
        ast as_tree;
        symbol_table s_table; // For reading/writing into
        ERRORS errors;

        std::vector<ast_node*> functions;
        std::stack<int> scope_stack;

        ast_node* process_function(ast_node* function);
        bool process_block(ast_node*& current);

        void process_assignment(ast_node*& current);
        void process_printf(ast_node*& current);

        ast_node* eval_top_three(std::string one, std::string two, std::string three);
        bool isOperator(std::string str);
        bool isNumber(std::string str);
        ast_node* getFunction(std::string str);
};  


#endif //CPLANE_INTERPRETER_H
