vector_test: vector_test.c vector.h
	gcc -std=c99 -pedantic -Wall -Wextra -o $@ $<
