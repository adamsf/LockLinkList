#include "list.h"
#include "listNoLocks.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define NUM_THREADS 30

struct linked_list *ll;

struct linked_list_lockless *lll;

void* thread_func(void* arg) {

    linked_list* ll = (linked_list*)arg;
    for (int i = 0; i < 100; i++) {
        ll_add(ll, i);
        assert(ll_contains(ll, i) > 0);
        ll_remove(ll, i);
    }
    return NULL;
}

void* thread_func2(void* arg) {

    linked_list_lockless* ll = (linked_list_lockless*)arg;
    for (int i = 0; i < 100; i++) {
        ll_add(ll, i);
        assert(ll_contains(ll, i) > 0);
        ll_remove(ll, i);
    }
    return NULL;
}


//Create test suite to test linked list lock saftey
int main(void)
{
    ll = ll_create();   
    pthread_t threads[NUM_THREADS];
    //printf("length of ll: %d\n", ll_length(ll));

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, &ll);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        //printf("Thread %d joined\n", i);
    }

    //printf("length %d\n", ll_length(ll));
    assert(ll_length(ll) == 0);
    ll_destroy(ll);

    printf("Linked list implementation is thread safe\n");
    return 0;
}

