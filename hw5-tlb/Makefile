CC = clang
CFLAGS = -Wall -g -Og
LDFLAGS = ""

libname = libmlpt.a
exec := test
headers := config.h tlb.h mlpt.h

$(libname): mlpt.o tlb.o
	ar rcs $@ $^

$(exec): test.o $(libname)
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

test.o: $(headers)
mlpt.o: $(headers)
tlb.o: $(headers)

.PHONY: all clean

all: $(exec)
	./$(exec)

clean:
	rm -r $(exec) $(libname) *.o