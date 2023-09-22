CC = clang
CFLAGS = ""
LDFLAGS = ""

exec := pagetable

$(exec): main.o
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

main.o: config.h mlpt.h

.PHONY: clean all check

check:
	$(CC) -Wall main.c -o $(exec)

clean:
	rm -r $(exec) *.o timings.txt