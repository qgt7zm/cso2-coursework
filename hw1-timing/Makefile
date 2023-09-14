exec := gettimings

$(exec): gettimings.o
	cc -o $@ $^

.c.o:
	cc -o $@ -c $<

.PHONY: clean all

clean:
	rm -r $(exec) test *.o