CC = gcc

execs := main_np main_p11 main_p12 main_p21 main_p22
arg1 := "/usr/bin/emacs-gtk"

all: $(execs)
	./main_np $(arg1)
	./main_p11 $(arg1)
	./main_p12 $(arg1)
	./main_p21 $(arg1)
	./main_p22 $(arg1)

main_np: openmpstarter.c
	gcc -fopenmp -O2 -o $@ $^ -lm

main_p11: openmp11.c
	gcc -fopenmp -O2 -o $@ $^ -lm

main_p12: openmp12.c
	gcc -fopenmp -O2 -o $@ $^ -lm

main_p21: openmp21.c
	gcc -fopenmp -O2 -o $@ $^ -lm

main_p22: openmp22.c
	gcc -fopenmp -O2 -o $@ $^ -lm

.PHONY: clean all

clean:
	rm $(execs) *.o
