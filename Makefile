build:
	gcc -Iinclude -Llib -Wall -std=c99 ./src/*.c -o game -lmingw32 -lSDL2main -lSDL2
