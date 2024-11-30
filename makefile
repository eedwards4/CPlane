# Compiler and flags
CC      = g++
CXXFLAGS = -g -std=c++14
LFLAGS  = 

# Source files and object files
SOURCE  = main.cpp errors.cpp tokenizer.cpp exec_path.cpp recursive_descent_parser.cpp symbol_table.cpp ast.cpp interpreter.cpp
OBJS    = $(SOURCE:.cpp=.o)
HEADER  = tokens.h errors.h tokenizer.h exec_path.h recursive_descent_parser.h symbols.h symbol_table.h ast.h interpreter.h
OUT     = cplane

# Default target
all: $(OUT)

# Link object files to create the output binary
$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $(OUT) $(LFLAGS)
	@rm -f $(OBJS)

# Compile source files into object files
%.o: %.cpp $(HEADER)
	$(CC) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(OUT)
