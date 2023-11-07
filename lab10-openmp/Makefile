CC = gcc
CFLAGS = -fopenmp -O2
# CFLAGS = -O2
LDFLAGS = -lm

header := openmpstarter.h
execs := main_np main_p11 main_p12 main_p21 main_p22
arg1 := "/usr/bin/emacs-gtk"
# arg1 := "./Makefile"

all: $(execs)
	./main_np $(arg1)
	./main_p11 $(arg1)
	./main_p12 $(arg1)
	./main_p21 $(arg1)
	./main_p22 $(arg1)

main_np: openmpstarter.c $(header)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

main_p11: openmp11.c $(header)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

main_p12: openmp12.c $(header)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

main_p21: openmp21.c $(header)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

main_p22: openmp22.c $(header)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

.PHONY: clean all

clean:
	rm $(execs) *.o
