# Compiler and flags
CC      = g++
CXXFLAGS = -g -std=c++11
LFLAGS  = 

# Source files and object files
SOURCE  = main.cpp tokens.cpp tokenizer.cpp exec_path.cpp errors.cpp
OBJS    = $(SOURCE:.cpp=.o)
HEADER  = tokens.h tokenizer.h exec_path.h errors.h
OUT     = cplane

# Default target
all: clean 

# Link object files to create the output binary
$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $(OUT) $(LFLAGS)

# Compile source files into object files
%.o: %.cpp $(HEADER)
	$(CC) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(OUT)
