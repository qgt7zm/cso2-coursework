#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int global = 0;

void handle_sigint(int id) {
    global += 1;
    printf("SIGINT: global=%d\n", global);
}

void Initialize() {
	printf("Initializing\n");
	for (int i = 0; i < 1000000000; i++) {}
	printf("Initialized\n");
}

void Process(char line[]) {
	printf("Processing");
	for (int i = 0; i < 1000000000; i++) {}
	printf("Processed");
}

int main() {
    Initialize();
    struct sigaction sa;
    sa.sa_handler = &handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);

    char line[128];
    while (fgets(line, sizeof line, stdin)) {
        Process(line);
    }
}