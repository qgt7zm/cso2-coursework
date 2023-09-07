objects := main.o split.o
exec := split

$(exec): $(objects)
	cc -o $@ $^

.c.o:
	cc -o $@ -c $<

.PHONY: clean all check

check:
	cc -Og -g -std=c11 -Wall -pedantic -o split main.c split.c -fsanitize=address

clean:
	rm $(exec) $(objects)