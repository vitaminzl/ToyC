compiler:main.o lexer.o
	g++ -o build/compiler main.o lexer.o -g -W
main.o:src/main.cpp src/lexer/lexer.h
	g++ -c src/main.cpp -g -W
lexer.o:src/lexer/lexer.cpp
	g++ -c src/lexer/lexer.cpp -g -W
clean:
	rm -rf *.o