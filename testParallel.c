#include "list.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 2

pthread_t thread1;
pthread_t thread2;
struct linked_list *ll;


void *firstThing(void *arg)
{
    printf("Thread 1\n");
    sched_yield();
    printf("Thread 1\n");
    return NULL;
}

void *secondThing(void *args){
    printf("Thread 2\n");
    return NULL;
}


//Create test suite to test linked list lock saftey
int main(void)
{
    pthread_create(&thread1, NULL, firstThing, NULL);
    pthread_create(&thread2, NULL, secondThing, NULL);
    ll = ll_create();
    printf("threads created");
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    

    return 0;
}

