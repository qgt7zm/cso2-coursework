CC = clang
CFLAGS = -Wall -g -Og
LDFLAGS = ""

exec := test
headers := config.h tlb.h mlpt.h cacheutil.h

$(exec): test.o tlb.o cacheutil.o mlpt.o
	$(CC) $(LDFLAGS) -o $@ $^

.c.o: $(headers)
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: all clean

all: $(exec)
	./$(exec)

clean:
	rm -r $(exec) *.o