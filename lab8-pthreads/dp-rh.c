#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>

pthread_t philosopher[5];
pthread_mutex_t chopstick[5];

/** Dining philosophers problem with resource hierarchy approach */
void *eat(void *arg) {
    int n = (int) (long)arg;

    // order the chopsticks by number
    int left = n;
    int right = (n+1)%5;

    int n1, n2;
    if (left < right) {
        n1 = left;
        n2 = right;
    } else {
        n1 = right;
        n2 = left;
    }

    // take two chopsticks
    pthread_mutex_lock(&chopstick[n1]);
    printf("Philosopher %d got chopstick %d\n", n, n1);
    pthread_mutex_lock(&chopstick[n2]);
    printf("Philosopher %d got chopstick %d\n", n, n2);
    
    printf("Philosopher %d is eating\n",n);
    sleep(1);
    
    // set them back down
    printf("Philosopher %d set down chopstick %d\n", n, n2);
    pthread_mutex_unlock(&chopstick[n2]);
    printf("Philosopher %d set down chopstick %d\n", n, n1);
    pthread_mutex_unlock(&chopstick[n1]);
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