#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <time.h>

long long getnsecs() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000000000 + t.tv_nsec;
}

int main(int argc, char *argv[]) {
	long long timeStart = getnsecs();
	printf("Hello there!\n");
	long long timeEnd = getnsecs();

	long long timeDiff = timeEnd - timeStart;
	printf("That took %lld nanoseconds!\n", timeDiff);
	printf("That took %lld microseconds!\n", timeDiff / 1000);
	printf("That took %f milliseconds!\n", timeDiff / 1000000.0);
	printf("That took %f seconds!\n", timeDiff / 1000000000.0);
	return 0;
}