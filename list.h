#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct Node Node; 
struct Node 
{
	int value;
	Node *next;
};


typedef struct linked_list
{
	Node *head;
	int size;
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

	if (ll->size == 0)
	{
		free(ll);
		return 1;
	} 
	return 0;
}

static inline void
ll_add(struct linked_list *ll, int value)
{
	if (ll == NULL) return;
	Node *temp = malloc(sizeof(Node));
	temp->value = value;
	temp->next = ll->head;
	ll->size += 1;
	ll->head = temp;
}

static inline int
ll_length(struct linked_list *ll)
{
	if (ll == NULL) return 0;
	return ll->size;
}

static inline bool
ll_remove_first(struct linked_list *ll)
{
	if (ll == NULL) return false;
	if (ll->size == 0) return false;
	Node *temp = ll->head;
	ll->head = ll->head->next;
	ll->size -= 1;
	free(temp);
	return true;
}

static inline int
ll_contains(struct linked_list *ll, int value)
{
	if (ll == NULL) return 0;
	if (ll->size == 0) return 0;
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
	return 0;
}

#endif
