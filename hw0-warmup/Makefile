objects := main.o split.o

all: $(objects)
	cc -o split $(objects)

main.o: main.c split.h
	cc -c main.c
split.o: split.c split.h
	cc -c split.c

.PHONY: clean all

clean:
	rm split $(objects)