#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>

pthread_t philosopher[5];
pthread_mutex_t chopstick[5];

int rand_int(int min, int max) {
    int range = max - min + 1;
    return (rand() % range) + min;
}

void release_chopstick(int p_index, int index) {
    printf("Philosopher %d set down chopstick %d\n", p_index, index);
    pthread_mutex_unlock(&chopstick[index]);
}

void think(int p_index, int *min_wait, int *max_wait) {
    int wait_us = rand_int(*min_wait, *max_wait);
    printf("Philosopher %d thinks for %d µs\n", p_index, wait_us);
    usleep(wait_us);

    // double next wait time
    *min_wait *= 2; 
    *max_wait *= 2;
}

/** Dining philosophers problem with backoff and retry approach */
void *eat(void *arg) {
    int n = (int) (long)arg;

    int min_wait = 50;
    int max_wait = 100;

    int has_lock1 = 0;
    int has_lock2 = 0;

    while (!(has_lock1 && has_lock2)) {
        // take two chopsticks
        if (pthread_mutex_trylock(&chopstick[n]) == 0) {
            printf("Philosopher %d got chopstick %d\n", n, n);
            has_lock1 = 1;
        } else {
            // backoff and retry
            think(n, &min_wait, &max_wait);
            continue;
        }
        
        if (pthread_mutex_trylock(&chopstick[(n+1)%5]) == 0) {
            printf("Philosopher %d got chopstick %d\n", n, (n+1)%5);
            has_lock2 = 1;
        } else {
            // set down the first chopstick
            release_chopstick(n, n);
            has_lock1 = 0;

            // backoff and retry
            think(n, &min_wait, &max_wait);
            continue;
        }
    }
    
    printf("Philosopher %d is eating\n",n);
    sleep(1);
    
    // set them back down
    release_chopstick(n, (n+1)%5);
    release_chopstick(n, n);
    return NULL;
}

int main(int argc, const char *argv[]) {
    for(int i = 0; i < 5; i += 1)
        pthread_mutex_init(&chopstick[i], NULL);

    for(int i =0; i < 5; i += 1)
        pthread_create(&philosopher[i], NULL, eat, (void *)(size_t)i);

    for(int i=0; i < 5; i += 1)
        pthread_join(philosopher[i], NULL);

    for(int i=0; i < 5; i += 1)
        pthread_mutex_destroy(&chopstick[i]);

    return 0;
}