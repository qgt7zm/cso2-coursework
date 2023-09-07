exec := split

$(exec): main.o split.o
	cc -o $@ $^

test: test.o split.o
	cc -o $@ $^
	./test

.c.o:
	cc -o $@ -c $<

.PHONY: clean all check

check:
	gcc -Wall -pedantic -std=c11 -Og -g -fsanitize=address main.c split.c -o $(exec)

clean:
	rm -r $(exec) test *.o split.dSYM