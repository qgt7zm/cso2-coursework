CC = clang
CFLAGS = ""
LDFLAGS = ""

exec := mysys

all: $(exec)
	./$(exec)

$(exec): main.o mysystem.o
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

mysystem.c: mysystem.h
main.c: mysystem.h

.PHONY: clean all

clean:
	rm $(exec) *.o