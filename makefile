all: el

el: *.c *.h
	gcc -g -o el *.c -lcairo -lm -ansi -Wall -Wpedantic

run: all
	./el

clean:
	rm el
