CC = clang

execs := dp dp-arb dp-rh dp-backoff

all: $(execs)

dp: dp.c
	$(CC) -O2 -lpthread -fsanitize=thread -o $@ $^

dp-arb: dp-arb.c
	$(CC) -O2 -lpthread -fsanitize=thread -o $@ $^

dp-rh: dp-rh.c
	$(CC) -O2 -lpthread -fsanitize=thread -o $@ $^

dp-backoff: dp-backoff.c
	$(CC) -O2 -lpthread -fsanitize=thread -o $@ $^

.PHONY: clean all

clean:
	rm $(execs) *.o