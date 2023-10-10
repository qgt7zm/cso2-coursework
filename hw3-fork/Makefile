exec := test

all: $(exec)
	./$(exec)

$(exec): test.o fork_run.o
	cc -o $@ $^

.c.o:
	cc -o $@ -c $<

.PHONY: all clean

clean:
	rm -r $(exec) $(libname) *.o