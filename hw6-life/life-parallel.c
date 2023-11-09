#include "life.h"

#include <stdlib.h>
#include <pthread.h>

// Helper Structs

typedef struct {
    pthread_barrier_t *barrier;
    int steps;
    int threads;
    int threads_completed;
    LifeBoard *state;
    LifeBoard *next_state;
} GameInfo;

typedef struct {
    int thread_id;
    GameInfo *game;
} ThreadInfo;

// Helper Functions

GameInfo *Game_create(int steps, int threads) {
    GameInfo *game = calloc(1, sizeof(GameInfo));

    // Initialize
    game->steps = steps;
    game->threads = threads;
    game->threads_completed = 0;

    // Create the barrier
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, threads);
    game->barrier = &barrier;

    return game;
}

void Game_delete(GameInfo *game) {
    pthread_barrier_destroy(game->barrier);
    LB_del(game->next_state);
    free(game);
}

ThreadInfo *Thread_create(int thread_id, GameInfo *game) {
    ThreadInfo *thread = calloc(1, sizeof(ThreadInfo));
    thread->thread_id = thread_id;
    thread->game = game;
    return thread;
}

void Thread_delete(ThreadInfo *thread) {
    free(thread);
}

// Life Functions

void *life_threaded(void *arg) {
    // GameInfo *game = (GameInfo *) arg;

    ThreadInfo *thread = (ThreadInfo *) arg;
    GameInfo *game = thread->game;

    for (int step = 0; step < game->steps; step++) {
        printf("Thread %d waiting\n", thread->thread_id);
        game->threads_completed += 1;
        pthread_barrier_wait(game->barrier);

        if (game->threads_completed == game->threads) {
            LB_swap(game->state, game->next_state);
            game->threads_completed = 0;
            printf("Finished step %d\n\n", step);
        }   
    }
    return NULL;
}

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {
    // Create the game argument
    GameInfo *game = Game_create(steps, threads);

    game->state = state;
    game->next_state = LB_new(state->width, state->height); // Save the state of the next step

    // Partition the board roughly evenly
    double slice_width_unrounded = (double) state->width / threads;
    int slice_width = (int) (slice_width_unrounded + 0.5); // Round to nearest integer
    if (slice_width == 0) slice_width = 1;
    
    printf("Total width = %d\n", state->width);
    printf("Threads = %d\n", threads);
    printf("Slice width = %.2f\n", slice_width_unrounded);

    // Create all threads and run the game
    ThreadInfo *thread_infos[threads];
    pthread_t game_threads[threads];
    for (int i = 0; i < threads; i++) {
        ThreadInfo *thread = Thread_create(i, game);
        thread_infos[i] = thread;
	    pthread_create(&game_threads[i], NULL, life_threaded, (void *) thread);
    }

    // Delete all threads
    for (int i = 0; i < threads; i++) {
	    pthread_join(game_threads[i], NULL);
        Thread_delete(thread_infos[i]);
    }

    // Free all resources
    Game_delete(game);
}
