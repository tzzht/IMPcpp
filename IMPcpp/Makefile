CC = g++

IMPComplier: parser.cpp scanner.cpp imp.cpp interpreter.cpp main.cpp
	$(CC) scanner.cpp parser.cpp imp.cpp interpreter.cpp main.cpp -o IMPComplier

scanner.cpp: scanner.l
	flex -o scanner.cpp scanner.l
	
parser.cpp: parser.y
	bison -o parser.cpp parser.y

clean:
	rm -rf scanner.cpp
	rm -rf parser.cpp parser.hpp location.hh position.hh stack.hh
	rm -rf a.out IMPComplier
	rm -rf *.dot *.png
