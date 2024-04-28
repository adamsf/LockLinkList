#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdatomic.h>


typedef struct Node Node; 
struct Node 
{
	int value;
	Node *next;
	bool deleted; //flag for checking if a node has been deleted in thread A before being referenced in thread B
};


typedef struct linked_list
{
	Node *head;
	int size;
	int ref_counter; //tracks how many threads are pointing to a linked list at once 
} linked_list;

static inline struct linked_list *
ll_create(void)
{
	linked_list* ll_ptr = malloc(sizeof(linked_list));
	if (ll_ptr == NULL)
	{
		return NULL;
	}
	ll_ptr->head = NULL;
	ll_ptr->size = 0;
	return ll_ptr;
}

static inline int
ll_destroy(struct linked_list *ll)
{
	if (ll == NULL) return 0;
	ll->ref_counter++;
	if (ll->size == 0)
	{
		free(ll);
		return 1;
	} 
	ll->ref_counter--;
	return 0;
}

static inline void
ll_add(struct linked_list *ll, int value)
{
	
	if (ll == NULL) return;
	ll->ref_counter++;
	Node *temp = malloc(sizeof(Node)); //moved lock below this
	temp -> value = value;
	temp->deleted = 0;
    do {
        temp -> next = ll -> head;
    }while(__sync_bool_compare_and_swap(&ll -> head, temp -> next, temp) == 0);
	ll->ref_counter--;
}

static inline int
ll_length(struct linked_list *ll)
{
	//should always give accurate response
	if (ll == NULL) return 0;
	ll->ref_counter++;
	int sz = ll->size;
	ll->ref_counter--;
	return sz;
}

//need to support removal from anywhere in the list not just front
static inline bool
ll_remove(struct linked_list *ll, int key)
{
	
	if (ll == NULL) return false;
	if (ll->size == 0) return false; //moved lock below this
	ll->ref_counter++;
	Node *temp = ll->head;
	if (temp->value == key)
	{
		ll->size -= 1;
		Node* prevtemp = temp;
		if (prevtemp->deleted == false)
		{
			temp = temp->next;
			prevtemp->deleted = true;
			free(prevtemp);
		}	
		return true;
	}
	while (temp->next != NULL)
	{
		Node* toRemove = temp->next;
		if (toRemove->value == key && toRemove->deleted == false)
		{
			temp->next = toRemove->next;
			ll->size -= 1;
			toRemove->deleted = true;
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
ll_contains(struct linked_list *ll, int value)
{
	
	if (ll == NULL) return 0;
	if (ll->size == 0) return 0; //moved lock below this
	ll->ref_counter++;
	Node* temp = ll->head;
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
