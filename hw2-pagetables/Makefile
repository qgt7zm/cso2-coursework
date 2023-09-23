CC = clang
CFLAGS = ""
LDFLAGS = ""

libname = libmlpt.a
exec := test
headers := config.h mlpt.h

$(libname): mlpt.o util.o
	ar rcs $@ $^	

$(exec): test.o $(libname)
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

test.o: config.h $(headers)
mlpt.o: config.h $(headers)
util.o: util.h

.PHONY: check clean run

# Check for compilation warnings
check: 
	make $(libname) CFLAGS="-Wall"

clean:
	rm -r $(exec) *.o

# Run the test code
run: $(exec)
	./$(exec)