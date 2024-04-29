#ifndef LISTNOLOCKS_H
#define LISTNOLOCKS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>


typedef struct Node Node_lockless; 
struct Node 
{
	int value;
	Node_lockless *next;
	bool deleted;
};


typedef struct linked_list_lockless
{
	Node_lockless *head;
	int size;
	int ref_counter;
} linked_list_lockless;

static inline struct linked_list_lockless *
lll_create(void)
{
	linked_list_lockless* ll_ptr = malloc(sizeof(linked_list_lockless));
	ll_ptr->ref_counter = 1;
	int cur_refs = ll_ptr->ref_counter;
	while (cur_refs != ll_ptr->ref_counter); //wait until other threads have finished
	if (ll_ptr == NULL)
	{
		return NULL;
	}
	ll_ptr->head = NULL;
	ll_ptr->size = 0;
	ll_ptr->ref_counter--;
	return ll_ptr;
}

static inline int
lll_destroy(struct linked_list_lockless *ll)
{
	if (ll == NULL) return 0;
	ll_ptr->ref_counter++;
	int cur_refs = ll_ptr->ref_counter;
	while (cur_refs != ll_ptr->ref_counter);
	if (ll->size == 0)
	{
		free(ll);
		return 1;
	} 
	ll_ptr->ref_counter--;
	return 0;
}

static inline void
lll_add(struct linked_list_lockless *ll, int value)
{
	
	if (ll == NULL) return;
	Node_lockless *temp = malloc(sizeof(Node)); //moved lock below this
	ll->ref_counter++;
	int cur_refs = ll->ref_counter;
	while (cur_refs != ll->ref_counter);
	temp -> value = value;
    do {
        temp -> next = ll -> head;
    }while(__sync_bool_compare_and_swap(&ll -> head, temp -> next, temp) == 0);
	ll->ref_counter--;
}

static inline int
lll_length(struct linked_list *ll)
{
	//should always give accurate response
	ll->ref_counter++;
	int cur_refs = ll->ref_counter;
	while (cur_refs != ll->ref_counter); //wait until other threads have finished
	int sz = ll->size;
	ll->ref_counter--;
}

//need to support removal from anywhere in the list not just front
static inline bool
lll_remove(struct linked_list *ll, int key)
{
	
	if (ll == NULL) return false;
	if (ll->size == 0) return false; //moved lock below this
	ll->ref_counter++;
	int cur_refs = ll->ref_counter;
	while (cur_refs != ll->ref_counter);
	Node_lockless *temp = ll->head;
	if (temp->value == key && temp->deleted == false)
	{
		ll->size -= 1;
		Node_lockless* prevtemp = temp;
		temp = temp->next;
		prevtemp->deleted = true;
		free(prevtemp);
		return true;
	}
	while (temp->next != NULL)
	{
		Node_lockless* toRemove = temp->next;
		if (toRemove->value == key && toRemove->deleted == false)
		{
			temp->next = toRemove->next;
			toRemove->deleted = true;
			ll->size -= 1;
			free(toRemove);
			return true;
		}
		temp = temp->next;
	}
	//the value was not removed 
	ll->ref_counter--;
	return false;
}


static inline int
lll_contains(struct linked_list_lockless *ll, int value)
{
	
	if (ll == NULL) return 0;
	if (ll->size == 0) return 0; //moved lock below this
	ll->ref_counter++;
	int cur_refs = ll->ref_counter;
	while (cur_refs != ll->ref_counter);
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
	ll->ref_counter--;
	return 0;
}

#endif
