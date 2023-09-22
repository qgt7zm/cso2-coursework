CC = clang
CFLAGS = ""
LDFLAGS = ""

libname = libmlpt.a
exec := pagetable
headers := config.h mlpt.h

$(libname): mlpt.o util.o
	ar rcs $@ $^

$(exec): main.o $(libname)
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

main.o: config.h $(headers)
mlpt.o: config.h $(headers)
util.o: util.h

.PHONY: check clean test

# Check for compilation warnings
check: 
	make $(libname) CFLAGS="-Wall"

clean:
	rm -r $(exec) *.o

# Run the test code
test: $(exec)
	./$(exec)