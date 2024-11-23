 /*

        // If both nodes are in the symbol table (HARD)
        if ( s_table.find_symbol(two->value) && s_table.find_symbol(three->value) ){
            // TODO Do whatever is necessary for two (retrive value/call function/etc)
            // Update three
            
            // TODO update three in symbol table
            //TEMPORARY
            expression_stack.push(three);
            expression_stack.push(two);
            expression_stack.push(one);
            return;
        } 
        // If one node is in the symbol table
        else if ( s_table.find_symbol(two->value) || s_table.find_symbol(three->value) ){
            // If three is a symbol 
            if ( s_table.find_symbol(three->value) ){
                // If two is a number
                std::cout << "here" << std::endl;
                if ( isNumber(two->value) ){
                    std::cout << "here2" << std::endl;
                    EvalOperatorUpdate(one, two, three);
                }
                // If two is not a number
                else if ( ! isNumber(two->value) ){
                    //TODO STRING?
                } 
                
                // Push result back into expression stack
                if ( ! expression_stack.empty() ){
                    expression_stack.push(three);
                }
                // TODO update three in s_table
                //s_table.update_symbol
                //expression_stack.push(three);
                //expression_stack.push(two);
                return;
            } 
            // TODO If two is symbol and three isnt (need to find out and do (HARD))
            else if ( s_table.find_symbol(two->value) ){
                // If three is a number
                if ( isNumber(three->value) ){
                    EvalOperatorUpdate(one, two, three);
                }
                else if ( ! isNumber(three->value) ){
                    //TODO STRING?
                }
                if ( ! expression_stack.empty() ){
                    expression_stack.push(three);
                }

                //expression_stack.push(three);
                //expression_stack.push(two);
                return;
            } else {
                // ERROR I belive
            }
            
        }
        // Else neither are in symbol table and both are numbers
        else if ( isNumber(three->value) && isNumber(two->value) ){
            // Check operator to modify values
            EvalOperatorUpdate(one, two, three);

            if ( ! expression_stack.empty() ){
                expression_stack.push(three);
            } 
        }
        */