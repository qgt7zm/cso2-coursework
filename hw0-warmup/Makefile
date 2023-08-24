objects := main.o split.o
exec := split

$(exec): $(objects)
	cc -o $@ $^

.c.o:
	cc -o $@ -c $<

.PHONY: clean all

clean:
	rm $(exec) $(objects)