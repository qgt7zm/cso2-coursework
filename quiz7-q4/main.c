#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct thread_data {
    int x;
    int *p;
};

void *thread_func(void *raw_ptr) {
    struct thread_data *td;
    td = (struct thread_data *)raw_ptr;
    *(td->p) += td->x;      /* LINE A */
    td->x = *(td->p);       /* LINE B */
    free(td);
    return &td->x;
}

int global_array[2] = {0, 0};

int main() {
    pthread_t threads[2];
    for (int x = 0; x < 2; ++x) {
        struct thread_data *td;
        td = malloc(sizeof(struct thread_data));
        td->x = x * 2;
        td->p = &global_array[x];
        int ret = pthread_create(&threads[x], NULL, thread_func, (void*) td);
        /* LINE C */
    }
    /* LINE D */
    for (int i = 0; i < 2; ++i) {
        void *raw_ptr;
        pthread_join(threads[i], &raw_ptr);
        int *p;
        p = (int*) raw_ptr;
        printf("got result %d\n", *p); /* LINE E */
        printf("global = %d\n", global_array[i]);
    }
}