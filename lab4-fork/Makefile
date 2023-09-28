CC = clang
CFLAGS = ""
LDFLAGS = ""

exec := test

all: $(exec)
	./$(exec)

$(exec): main.o my_system.o
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

my_system.o: my_system.h
main.o: my_system.h

.PHONY: clean all

clean:
	rm $(exec) *.o