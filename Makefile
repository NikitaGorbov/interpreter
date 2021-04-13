.DEFAULT_GOAL := g++
CFLAGS = -Wall -Iinclude

g++: CC = g++ 
g++: bin obj bin/interpreter

debug: CC = g++
debug: CFLAGS += -pedantic -fsanitize=leak,address
debug: bin obj bin/interpreter

obj: bin/const.o bin/lexem.o bin/main.o bin/lexical.o bin/semantic.o bin/syntax.o

bin/const.o: src/const.cpp
	$(CC) -c src/const.cpp -o bin/const.o $(CFLAGS)

bin/lexem.o: src/lexem.cpp
	$(CC) -c src/lexem.cpp -o bin/lexem.o $(CFLAGS)

bin/main.o: src/main.cpp
	$(CC) -c src/main.cpp -o bin/main.o $(CFLAGS)

bin/lexical.o: src/lexical.cpp
	$(CC) -c src/lexical.cpp -o bin/lexical.o $(CFLAGS)

bin/semantic.o: src/semantic.cpp
	$(CC) -c src/semantic.cpp -o bin/semantic.o $(CFLAGS)

bin/syntax.o: src/syntax.cpp
	$(CC) -c src/syntax.cpp -o bin/syntax.o $(CFLAGS)

bin/interpreter: bin/const.o bin/lexem.o bin/main.o bin/lexical.o bin/semantic.o bin/syntax.o
	$(CC) bin/main.o bin/const.o bin/lexem.o bin/lexical.o bin/semantic.o bin/syntax.o $(CFLAGS) -o bin/interpreter

bin:
	mkdir -p bin

clean:
	rm -rf bin
