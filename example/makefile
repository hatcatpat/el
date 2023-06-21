all: test

test: test.c
	gcc -o test test.c ../el.c -lcairo -lm -ansi -Wall -Wpedantic

run: all
	./test

clean:
	rm test
