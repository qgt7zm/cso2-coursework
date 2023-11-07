CC = clang

exec := main

all: $(exec)
	./$(exec) "/usr/bin/emacs-gtk"

$(exec): openmpstarter.c
	gcc -fopenmp -O2 -o $@ $^ -lm

.PHONY: clean all

clean:
	rm $(exec) *.o
