exec := pagetable

$(exec): main.o
	cc -o $@ $^

.c.o:
	cc -o $@ -c $<

main.o: config.h mlpt.h

.PHONY: clean all check

check:
	gcc -Wall main.c -o $(exec)

clean:
	rm -r $(exec) *.o timings.txt