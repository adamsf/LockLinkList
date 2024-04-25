#include "list.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define NUM_THREADS 2

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

void *secondThing(){
    //ll_remove_first(ll);
    return NULL;
}


//Create test suite to test linked list lock saftey
int main(void)
{
    ll = ll_create();
    pthread_create(&thread1, NULL, firstThing, NULL);
    pthread_create(&thread2, NULL, secondThing, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("LL contains 1: %d\n", ll_contains(ll, 3));



    

    return 0;
}

