exec := gettimings

$(exec): gettimings.o
	cc -o $@ $^

.c.o:
	cc -o $@ -c $<

gettimings.o: funcs.h handlers.h timer.h

.PHONY: clean all

clean:
	rm -r $(exec) *.o