CC = clang
CFLAGS = ""
LDFLAGS = "-O2 -lpthread"

exec := dp

all: $(exec)
	./$(exec)

$(exec): dp.o
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean all

clean:
	rm $(exec) *.o