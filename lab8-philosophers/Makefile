CC = clang

execs := dp dp-arb

all: $(execs)

dp: dp.c
	$(CC) -O2 -lpthread -fsanitize=thread -o $@ $^

dp-arb: dp-arb.c
	$(CC) -O2 -lpthread -fsanitize=thread -o $@ $^

.PHONY: clean all

clean:
	rm $(execs) *.o