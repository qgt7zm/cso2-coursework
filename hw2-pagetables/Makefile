CC = clang
CFLAGS = ""
LDFLAGS = ""

libname = libmlpt.a
exec := test
headers := config.h mlpt.h allocate.o translate.h

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

.PHONY: all check clean

# Run the test code
all: $(exec)
	./$(exec)

# Check for compilation warnings
check: 
	make $(libname) CFLAGS="-Wall"

clean:
	rm -r $(exec) $(libname) *.o