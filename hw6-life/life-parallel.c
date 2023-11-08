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

// Helper Functions

GameInfo *Game_create(int steps, int threads) {
    GameInfo *game = calloc(1, sizeof(GameInfo));

    // Initialize
    game->steps = steps;
    game->threads = threads;
    game->threads_completed = 0;

    return game;
}

void Game_delete(GameInfo *game) {
    free(game);
}

void *life_threaded(void *arg) {
    GameInfo *game = (GameInfo *) arg;

    for (int step = 0; step < game->steps; step++) {
        printf("Waiting step %d\n", step);
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
    // Create the barrier
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, threads);

    // Partition the board roughly evenly
    int slice_width = state->width / threads;
    if (slice_width == 0) slice_width = 1;

    // Step up the function argument
    GameInfo *game = Game_create(steps, threads);
    game->barrier = &barrier;

    LifeBoard *next_state = LB_new(state->width, state->height); // Save the state of the next step
    game->state = state;
    game->next_state = next_state;

    // Create all threads and run the game
    pthread_t game_threads[threads];
    for (int i = 0; i < threads; i++) {
	    pthread_create(&game_threads[i], NULL, life_threaded, (void *) game);
    }

    // Delete all threads
    for (int i = 0; i < threads; i++) {
	    pthread_join(game_threads[i], NULL);
    }

    // Free all resources
    pthread_barrier_destroy(&barrier);
    LB_del(next_state);
    Game_delete(game);
}
