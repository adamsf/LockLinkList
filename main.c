#include <list.h>
#include <assert.h>

linked_list *ll;

//TODO save with tar -czvf Adams_HW0.tar.gz list.h main.c Makefile

int main(void)
{	
	//test null pointer with all functions
	assert(ll == NULL);
	ll_add(ll, 23);
	assert(ll_contains(ll, 23) == 0);
	assert(ll_remove_first(ll) == false);
	assert(ll_destroy(ll) == 0);
	assert(ll_length(ll) == 0);

	//test creation. not sure how to test a malloc failure 
	//without filling my memory
	ll = ll_create();	
	assert(ll != NULL);
	assert(ll->head == NULL);
	assert(ll->size == 0);

	//test destroy, then recreation
	assert(ll_destroy(ll) == 1);
	

	ll = ll_create();	
	assert(ll != NULL);
	assert(ll->head == NULL);
	assert(ll->size == 0);
	
	//test length func
	assert(ll_length(ll) == 0);


	//test everything except add with an empty list (head should be null)
	assert(ll_remove_first(ll) == false);
	assert(ll_contains(ll, 123) == 0);

	//test add, empty, add, to ensure no bugs
	ll_add(ll, 1);
	assert(ll_length(ll) == 1);
	assert(ll_contains(ll, 1) == 1);

	ll_remove_first(ll);
	assert(ll != NULL && ll_length(ll) == 0);

	ll_add(ll, 2);
	assert(ll_length(ll) == 1);

	//build list of 4->-7->23->2
	ll_add(ll, 23);
	assert(ll_length(ll) == 2);
	ll_add(ll, -7);
	assert(ll_length(ll) == 3);
	ll_add(ll, 4);
	assert(ll_length(ll) == 4);

	//test remove with non-singleton list
	//list should be 23->2
	ll_remove_first(ll);
	assert(ll_length(ll) == 3);
	ll_remove_first(ll);
	assert(ll_length(ll) == 2);
	
	//test contains with true on first, second, fourth items
	//add items 
	assert(ll_contains(ll, 7) == 0);
	ll_add(ll, 6);
	ll_add(ll, -9);

	assert(ll_contains(ll, -9) == 1);
	assert(ll_contains(ll, 6) == 2);
	assert(ll_contains(ll, 2) == 4);

	//tear down and destroy ll pointer
	//try to remove with elements in list first 
	assert(ll_destroy(ll) == 0);

	while (ll_length(ll) > 0)
	{
		ll_remove_first(ll);
	}
	assert(ll_length(ll) == 0);
	assert(ll_destroy(ll) == 1);

	ll = NULL;

	return 0;
}
