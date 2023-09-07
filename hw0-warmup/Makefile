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
	cc -Og -g -std=c11 -Wall -pedantic -o split main.c split.c -fsanitize=address

clean:
	rm -r $(exec) test *.o split.dSYM