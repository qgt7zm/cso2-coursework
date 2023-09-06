CC = clang
CFLAGS = ""
LDFLAGS = ""

exec := chat

all: $(exec)

# Build executable
$(exec): chat.o
	$(CC) $(LDFLAGS) -o $@ $^

# Compile source to object
.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean all

clean:
	rm $(exec) *.o