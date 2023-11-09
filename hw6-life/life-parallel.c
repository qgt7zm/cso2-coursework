#include "life.h"

#include <stdlib.h>
#include <pthread.h>

// Helper Structs

typedef struct {
    pthread_barrier_t *barrier;
    int steps;
    int threads;
    LifeBoard *state;
    LifeBoard *next_state;
} GameInfo;

typedef struct {
    int thread_id;
    GameInfo *game;
    int startX; // Start column
    int endX; // End column, exclusive
} ThreadInfo;

// Helper Functions

GameInfo *Game_create(int steps, int threads) {
    GameInfo *game = calloc(1, sizeof(GameInfo));

    // Initialize fields
    game->steps = steps;
    game->threads = threads;

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

ThreadInfo *Thread_create(int thread_id, GameInfo *game, int startX, int endX) {
    ThreadInfo *thread = calloc(1, sizeof(ThreadInfo));

    thread->thread_id = thread_id;
    thread->game = game;
    thread->startX = startX;
    thread->endX = endX;
    
    return thread;
}

void Thread_delete(ThreadInfo *thread) {
    free(thread);
}

// Life Functions

void partition_board(int board_width, int threads, int starts[], int ends[]) {
    double slice_width_unrounded = (double) board_width / threads;
    int slice_width = (int) (slice_width_unrounded + 0.5); // Round to nearest integer
    if (slice_width == 0) slice_width = 1;
    
    // printf("Total width = %d\n", board_width);
    // printf("Threads = %d\n", threads);
    // printf("Slice width = %.2f\n", slice_width_unrounded);

    int current = 0;
    for (int i = 0; i < threads; i++) {  
        if (i == 0) starts[i] = 1; // Start at 1
        else starts[i] = current;

        ends[i] = current;
        if (i == threads - 1) ends[i] = board_width - 1; // End at width - 1
        else ends[i] = current + slice_width;

        current += slice_width;
    }
    // printf("\n");
}

void *life_threaded(void *arg) {
    // GameInfo *game = (GameInfo *) arg;

    ThreadInfo *thread = (ThreadInfo *) arg;
    GameInfo *game = thread->game;
    LifeBoard *state = game->state;
    LifeBoard *next_state = game->next_state;
    // printf("Slice %d: [%d, %d)\n", thread->thread_id, thread->startX, thread->endX);

    for (int step = 0; step < game->steps; step++) {
        for (int y = 1; y < state->height - 1; y++) {
            for (int x = thread->startX; x < thread->endX && x < state->width - 1; x++) {
                // printf("- Slice %d, Cell (%d, %d)\n", thread->thread_id, x, y);

                // Count live neighbors
                int live_in_window = 0;
                for (int y_offset = -1; y_offset <= 1; y_offset += 1)
                    for (int x_offset = -1; x_offset <= 1; x_offset += 1)
                        if (LB_get(state, x + x_offset, y + y_offset))
                            live_in_window += 1;
                
                // Set next state
                // TODO race condition
                LB_set(next_state, x, y,
                    live_in_window == 3 /* dead cell with 3 neighbors or live cell with 2 */ ||
                    (live_in_window == 4 && LB_get(state, x, y)) /* live cell with 3 neighbors */
                );

            }
        }

        // Wait until all threads are done
        pthread_barrier_wait(game->barrier);

        // Have first thread swap states
        if (thread->thread_id == 0) {
            LB_swap(game->state, game->next_state);
            // printf("Finished step %d\n", step);
        }
        
        // Don't continue until states swapped
        pthread_barrier_wait(game->barrier);
    }

    return NULL;
}

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {
    // Create the game argument
    GameInfo *game = Game_create(steps, threads);

    game->state = state;
    game->next_state = LB_new(state->width, state->height); // Save the state of the next step


    // Partition the board roughly evenly
    int starts[threads];
    int ends[threads];
    partition_board(state->width, threads, starts, ends);

    // Create all threads and run the game
    ThreadInfo *thread_infos[threads];
    pthread_t game_threads[threads];
    for (int i = 0; i < threads; i++) {
        ThreadInfo *thread = Thread_create(i, game, starts[i], ends[i]);
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
