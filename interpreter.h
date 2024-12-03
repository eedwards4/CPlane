#ifndef CPLANE_INTERPRETER_H
#define CPLANE_INTERPRETER_H

#include "symbol_table.h"
#include "ast.h"
#include "errors.h"

#include <stack>
#include <type_traits>
#include <cctype>
#include <cmath>

class Interpreter {
    public:
        Interpreter();
        Interpreter(ast tree, symbol_table& table, ERRORS& errors);

        void begin(bool debug_mode = false);
    
    private:
        ast as_tree;
        symbol_table s_table; // For reading/writing into
        ERRORS errors;

        bool debug;

        std::vector<ast_node*> functions;
        std::stack<int> scope_stack;

        ast_node* process_function(ast_node* function);
        bool process_block(ast_node*& current);

        ast_node* process_call(ast_node*& current);

        void process_assignment(ast_node*& current);
        void process_if(ast_node*& current);
        void process_while(ast_node*& current);
        void process_for(ast_node*& current);
        void process_printf(ast_node*& current);

        void skip_block(ast_node*& current);

        ast_node* eval_top_three(std::string one, std::string two, std::string three);
        bool eval_bool_expression(ast_node*& current);
        std::string convert_escape_chars(std::string str);

        bool isOperator(std::string str);
        bool isNumber(std::string str);

        ast_node* getFunction(std::string str);

};  


#endif //CPLANE_INTERPRETER_H
