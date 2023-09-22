CC = clang
CFLAGS = ""
LDFLAGS = ""

libname = libmlpt.a
exec := test
headers := config.h mlpt.h

$(libname): mlpt.o util.o
	ar rcs $@ $^	

# Run the test code
$(exec): test.o $(libname)
	$(CC) $(LDFLAGS) -o $@ $^
	./$(exec)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

test.o: config.h $(headers)
mlpt.o: config.h $(headers)
util.o: util.h

.PHONY: check clean

# Check for compilation warnings
check: 
	make $(libname) CFLAGS="-Wall"

clean:
	rm -r $(exec) *.o