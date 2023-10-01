CC = clang
CFLAGS = -Wall -g -Og
LDFLAGS = ""

libname = libmlpt.a
exec := test
headers := config.h mlpt.h allocate.h translate.h

$(libname): mlpt.o translate.o allocate.o
	ar rcs $@ $^

$(exec): test.o $(libname)
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

test.o: $(headers)
mlpt.o: $(headers)
allocate.o: $(headers)
translate.o: $(headers)

.PHONY: all clean

# Run the test code
all: $(exec)
	./$(exec)

clean:
	rm -r $(exec) $(libname) *.o