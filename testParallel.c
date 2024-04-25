#include "list.h"
//#include "listNoLocks.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>


#define NUM_THREADS 10

pthread_t thread1;
pthread_t thread2;
struct linked_list *ll;



void *firstThing()
{
    ll_add(ll, 1); //locks here so context switch to thread 2
    ll_add(ll, 2);
    sched_yield();
    ll_add(ll, 3);
    sched_yield();
    printf("Thread 1\n");
    return NULL;
}

void* thread_func(void* arg) {

    printf("running thread\n");
    linked_list* ll = (linked_list*)arg;
    ll_add(ll, 1);
    ll_add(ll, 2);
    ll_add(ll, 3);
    ll_contains(ll,1);
    ll_remove(ll, 1);
    ll_remove(ll, 2);
    assert(ll_contains(ll, 3) == 1);
    assert(ll_contains(ll, 1) == 0);
    assert(ll_contains(ll, 2) == 0);
    
    return NULL;
}


//Create test suite to test linked list lock saftey
int main(void)
{
    ll = ll_create();   
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, &ll);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d joined\n", i);
    }


    

    return 0;
}

