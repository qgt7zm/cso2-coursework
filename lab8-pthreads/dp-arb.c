#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>

pthread_t philosopher[5];
pthread_mutex_t chopstick[5];
pthread_mutex_t arbitrator;

/** Dining philosophers problem with arbitrator approach */
void *eat(void *arg) {
    int n = (int) (long)arg;

    // call the waiter
    pthread_mutex_lock(&arbitrator);

    // take two chopsticks
    pthread_mutex_lock(&chopstick[n]);
    printf("Philosopher %d got chopstick %d\n", n, n);
    pthread_mutex_lock(&chopstick[(n+1)%5]);
    printf("Philosopher %d got chopstick %d\n", n, (n+1)%5);
    
    printf("Philosopher %d is eating\n",n);
    sleep(1);
    
    // set them back down
    printf("Philosopher %d set down chopstick %d\n", n, (n+1)%5);
    pthread_mutex_unlock(&chopstick[(n+1)%5]);
    printf("Philosopher %d set down chopstick %d\n", n, n);
    pthread_mutex_unlock(&chopstick[n]);

    // dismiss the waiter
    pthread_mutex_unlock(&arbitrator);
    return NULL;
}

int main(int argc, const char *argv[]) {
    for(int i = 0; i < 5; i += 1)
        pthread_mutex_init(&chopstick[i], NULL);
    pthread_mutex_init(&arbitrator, NULL);

    for(int i =0; i < 5; i += 1)
        pthread_create(&philosopher[i], NULL, eat, (void *)(size_t)i);

    for(int i=0; i < 5; i += 1)
        pthread_join(philosopher[i], NULL);

    for(int i=0; i < 5; i += 1)
        pthread_mutex_destroy(&chopstick[i]);
    pthread_mutex_destroy(&arbitrator);

    return 0;
}