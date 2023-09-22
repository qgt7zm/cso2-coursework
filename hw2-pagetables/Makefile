CC = clang
CFLAGS = ""
LDFLAGS = ""

exec := pagetable

$(exec): main.o util.o
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

main.o: config.h mlpt.h util.h
util.o: util.h

.PHONY: clean all check

check:
	$(CC) -Wall main.c -o $(exec)

clean:
	rm -r $(exec) *.o