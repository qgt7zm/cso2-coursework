#include "life.h"
#include <pthread.h>

struct life_info {
    pthread_barrier_t barrier;
    int steps;
    int threads;
    int threads_completed;
    LifeBoard *state;
    LifeBoard *next_state;
};

void *life_threaded(void *arg) {
    struct life_info *info = (struct life_info *) arg;

    for (int step = 0; step < info->steps; step++) {
        printf("- Waiting step %d\n", step);
        info->threads_completed += 1;
        pthread_barrier_wait(&info->barrier);

        if (info->threads_completed == info->threads) {
            LB_swap(info->state, info->next_state);
            info->threads_completed = 0;
            puts("Swapped states");
        }   
    }
    return NULL;
}

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {
    // Create the barrier
    struct life_info info;
    pthread_barrier_init(&info.barrier, NULL, threads);

    // Partition the board roughly evenly
    int slice_width = state->width / threads;
    if (slice_width == 0) slice_width = 1;

    // Step up the function argument
    info.steps = steps;
    LifeBoard *next_state = LB_new(state->width, state->height); // Save the state of the next step
    info.next_state = next_state;
    info.threads = threads;
    info.threads_completed = 0;
    info.state = state;
    info.next_state = next_state;

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
