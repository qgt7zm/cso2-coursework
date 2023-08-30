CC = clang
CFLAGS = ""
LDFLAGS = ""

exec := guesser
lib_src := cheer.o grunt.o

all: $(exec)

# Build executable
$(exec): guesser.o libsay.a
	$(CC) $(LDFLAGS) -o $@ $^

# Build static library
libsay.a: $(lib_src)
	ar rcs $@ $^

# Compile source to object
.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

cheer.o: cheer.c say.h
grunt.o: grunt.c say.h
guesser.o: guesser.c say.h

.PHONY: clean all

clean:
	rm $(exec) libsay.a guesser.o $(lib_src)