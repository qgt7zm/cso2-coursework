#include "life.h"
#include <pthread.h>

struct life_info {
    pthread_barrier_t barrier;
    int steps;
};

void *life_threaded(void *args) {
    struct life_info *info = (struct life_info *) args;

    for (int step = 0; step < info->steps; step++) {
        pthread_barrier_wait(&info->barrier);
    }
    return NULL;
}

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {
    // Create the barrier
    struct life_info info;
    pthread_barrier_init(&info.barrier, NULL, threads);

    LifeBoard *next_state = LB_new(state->width, state->height); // Save the state of the next step

    // Partition the board roughly evenly
    int slice_width = state->width / threads;
    if (slice_width == 0) slice_width = 1;
    
    // Create all threads and run the game
    pthread_t game_threads[threads];
    for (int i = 0; i < threads; i++) {
	pthread_create(&game_threads[i], NULL, life_threaded, (void *) &info);
    }

    // Delete all threads
    for (int i = 0; i < threads; i++) {
	pthread_join(game_threads[i], NULL);
    }

    // Free all resources
    pthread_barrier_destroy(&info.barrier);
    LB_del(next_state);
}
