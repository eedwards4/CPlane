all: main.o tokenizer.o recursive_descent_parser.o symbol_table.o exe clean

exe:
	g++ -g main.o symbol_table.o recursive_descent_parser.o tokenizer.o -o main

main.o:
	g++ -c main.cpp

tokenizer.o:
	g++ -c tokenizer.cpp

recursive_descent_parser.o:
	g++ -c recursive_descent_parser.cpp

symbol_table.o:
	g++ -c symbol_table.cpp

clean:
	rm main.o
	rm tokenizer.o
	rm recursive_descent_parser.o
	rm symbol_table.o