all: el

test: *.c *.h
	gcc -o test *.c -lcairo -lm -ansi -Wall -Wpedantic

run: all
	./test

clean:
	rm test
