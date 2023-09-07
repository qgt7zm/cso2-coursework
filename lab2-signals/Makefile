CC = clang
CFLAGS = ""
LDFLAGS = ""

exec := chat

all: $(exec)

# Build executable
$(exec): chat.o util.o
	$(CC) $(LDFLAGS) -o $@ $^

# Compile source to object
.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

chat.o: chat.c chat.h
util.o: util.c chat.h

.PHONY: clean all

clean:
	rm $(exec) *.o