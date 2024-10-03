all: generate.o
	./generate.o

generate.o: generate.c
	gcc generate.c -lm -o generate.o
	
