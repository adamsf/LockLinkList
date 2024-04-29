#ifndef LISTNOLOCKS_H
#define LISTNOLOCKS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdatomic.h>


typedef struct Node_lockless Node_lockless; 
struct Node_lockless
{
	int value;
	Node_lockless *next;
	bool deleted;
	int ref_counter;
};


typedef struct linked_list_lockless
{
	Node_lockless *head;
	atomic_int size;
} linked_list_lockless;

static inline struct linked_list_lockless *
lll_create(void)
{
	linked_list_lockless* ll_ptr = malloc(sizeof(linked_list_lockless));
	if (ll_ptr == NULL)
	{
		return NULL;
	}
	ll_ptr->head = NULL;
	atomic_init(&ll_ptr->size, 0);
	return ll_ptr;
}

static inline int
lll_destroy(struct linked_list_lockless *ll)
{
	if (ll == NULL) return 0;
	if (ll->size == 0)
	{
		free(ll);
		return 1;
	} 
	return 0;
}

static inline void
lll_add(struct linked_list_lockless *ll, int value)
{
	
	if (ll == NULL) return;
	Node_lockless *temp = malloc(sizeof(Node_lockless)); //moved lock below this
	temp->ref_counter = 1;
	temp->deleted = false;
	temp -> value = value;
    do {
        temp -> next = ll -> head;
    }while(__sync_bool_compare_and_swap(&ll -> head, temp -> next, temp) == 0);
}

static inline int
lll_length(struct linked_list_lockless *ll)
{
	//should always give accurate response
	if (ll == NULL) return 0;
	return atomic_load(&ll->size);
}

//need to support removal from anywhere in the list not just front
static inline bool
lll_remove(struct linked_list_lockless *ll, int key)
{
	
	if (ll == NULL) return false;
	if (atomic_load(&ll->size) == 0) return false;
	
	Node_lockless *temp = ll->head;
	Node_lockless *prev = NULL;

	while (temp != NULL && temp->deleted == true)
	{
		if(temp->value == key && temp->deleted == false)
		{
			if(prev == NULL) //removing head
			{
				if (__sync_bool_compare_and_swap(&ll->head, temp, temp->next)){
					atomic_fetch_sub(&ll->size, 1);
					temp->deleted = true;
					free(temp);
					return true;
				}
			}
			else
			{
				if(__sync_bool_compare_and_swap(&prev->next, temp, temp->next))
				{
					atomic_fetch_sub(&ll->size, 1);
					temp->deleted = true;
					free(temp);
					return true;
				}
			}
		}
	}
	//the value was not removed 
	return false;
}


static inline int
lll_contains(struct linked_list_lockless *ll, int value)
{
	
	if (ll == NULL) return 0;
	if (ll->size == 0) return 0; //moved lock below this
	
	Node_lockless* temp = ll->head;
	int ctr = 1;
	while (temp != NULL)
	{
		if (temp->value == value)
		{
			return ctr;
		}
		else 
		{
			temp = temp->next;
			ctr++;
		} 
	}
	return 0;
}

#endif
