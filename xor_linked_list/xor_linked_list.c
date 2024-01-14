/*
 ============================================================================
 Name        : CWK2Q4.c
 Author      : Anonymous (DO NOT CHANGE)
 Description :
 Implement your own XOR Linked List (https://en.wikipedia.org/wiki/XOR_linked_list)
 in C capable of storing names. Your implementation should have the following
 functions:
    void insert_string(const char* newObj)
	int insert_before(const char* before, const char* newObj)
	int insert_after(const char* after, const char* newObj)
	void remove_string(char* result)
	int remove_after(const char *after, char *result)
	int remove_before(const char *before, char *result)
    void print_list()

 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//https : //www.geeksforgeeks.org/xor-linked-list-a-memory-efficient-doubly-linked-list-set-2/
// I used this website to get a baseline structure for storing the nodes and XOR function and learning how traversal works
typedef struct node
{
	char *data;
	struct node *npx;

} node;

node *head = NULL;

struct node *XOR(struct node *x, struct node *y)
{

	return (struct node *)(((uintptr_t)x ^ (uintptr_t)y));
}

void print_list()
{
	node *current = head;
	node *previous = NULL;
	node *nxt;
	while (current != NULL)
	{
		printf("%s -> ", current->data);
		nxt = XOR(previous, current->npx);
		previous = current;
		current = nxt;
	}
	printf(" NULL\n");
}

node *specificRemove(int position, char *result)
{
	node *curr = head;
	node *prev = NULL;
	node *next = XOR(prev, curr->npx);
	int Pos = 1;
	while (next != NULL && Pos < position)
	{
		prev = curr;
		curr = next;
		next = XOR(prev, curr->npx);
		Pos++;
	}
	if (Pos == position && position != 1 )
	{
		// check they arent adding to the end as then there is no next
		if (next != NULL)
		{
			// update the next pointer to its next pointer and the previous one 
			// to the item being removed to skip the item being removed
			next->npx= XOR(prev, XOR(next->npx, curr));
		}
		// same idea as the next , but for the beginning as there would be no previous at the beggining. 
		if(prev != NULL){
			prev->npx = XOR(XOR(curr, prev->npx), next);
		}	
	}
	// if they are removing the head
	else if (position == 1){
		// update with NULL as there is no previous
		next->npx = XOR(NULL, XOR(next->npx, curr));

		// update head to be next as they are removing the head
		head = next;
	}
	// return the data stored in the item to remove to result
	strcpy(result, curr->data);

	// free the memory of current node that has just been removed.
	curr->data = NULL;
	curr->npx = NULL;
	free(curr);
}

node *specificInsert(const char *newObj, int position)
{
	node *curr = head;
	node *prev = NULL;
	node *next = XOR(prev, curr->npx);
	int Pos = 1;

	// traverse and locate the item at the current position
	while (next != NULL && Pos < position - 1)
	{
		
		prev = curr;
		curr = next;
		// calculate the next  item by XOR of the item before and the current pointer
		next = XOR(prev, curr->npx);
		// increment position, until its on the correct location
		Pos++;
	}

	if (Pos == position - 1)
	{

		// create new node
		struct node *create_node = (struct node *)malloc(sizeof(struct node));

		//current and next are the adjacnet nodes therefore npx of the new item would be XOR of current and next
		create_node->npx = XOR(curr, next);
		// set the data into the corret position.
		create_node->data = newObj;
		// set pointer of current to now have its address of the item before it which was stored in temp
		// and of course between hte new node that you have just created.
		curr->npx = XOR(XOR(next, curr->npx), create_node);
		// if you are inserting into the item at the end of the list, this can just stay unset as its null.
	
		if (next != NULL)
		{
			// this is to set the item that is after the new item to have its pointer to be added to create node.
			// if it was the end of the list this would be new item XOR 0.
			next->npx = XOR(create_node, XOR(next->npx, curr));
		}
	}

	// if inserting at the head
	else if (position == 1)
	{
		// create new node
		struct node *create_node = (struct node *)malloc(sizeof(struct node));

		// set the current pointer to be of the new node at the head and whatever it was as the head previously pointing to curr->npx
		curr->npx = XOR(create_node, XOR(NULL, curr->npx));

		//set the new node to point to the previous head
		create_node->npx = XOR(NULL, curr);

		// set this node as the new head
		head = create_node;

		// add its data.
		create_node->data = newObj;
	}
}

// locates the position in the list of a specific item that has been passed in
int present(const char *item)
{
	node *current = head;
	node *previous = NULL;
	node *nxt;
	int pos = 1;
	while (current != NULL)
	{
		// compares the data to check whether to return the postion that has been found.
		if (strcmp(current->data, item) == 0)
		{
			return pos;
		}
		nxt = XOR(previous, current->npx);
		previous = current;
		current = nxt;
		pos++;
	}
	return 0;
}
// just inserts a string basically
void insert_string(const char *newObj)
{


	// create the node
	struct node *create_node = (struct node *)malloc(sizeof(struct node));

	// add its data
	create_node->data = (char *)newObj;

	// set its pointer to be between the current head and NULL
	create_node->npx = XOR(NULL, head);

	// if there was a head already present
	if (head)
	{
		// set its pointer to now be pointing to what it was already but as its after the new head it is XOR'ed with create node

		(head)->npx = XOR(create_node, XOR((head)->npx, NULL));
	}

	// update the head to be this new node

	head = create_node;
}

// inserts just before the item stored in before
int insert_before(const char *before, const char *newObj)
{
	int pos;

	// locate the item to place before
	pos = present(before);
	fflush(stdout);
	if (pos)
	{
		// insert before that item which is just replace the item to be added at the same location it was found at.
		specificInsert(newObj, pos);
		// return 1 as it was a valid item 
		return 1;
	}
	else
	{
		printf("ERROR INSERTING");
		return 0;
	}
}

// same as the above function but pos + 1 as its inserting after 
int insert_after(const char *after, const char *newObj)
{
	int pos;
	pos = present(after);
	fflush(stdout);
	if (pos)
	{
		specificInsert(newObj, pos + 1);
		return 1;
	}
	else
	{
		printf("ERROR INSERTING");
		return 0;
	}
}


int remove_string(char *result)
{

    // if there are no items then return a failure
	if (!head)
	{
		return 0;
	}
	
	// otherwise return the item at the front and place into result
	specificRemove(1, result);
	return 1;
}

// remove after would take the pos + 1 of the item they wish to remove after 
int remove_after(const char *after, char *result)
{
	int pos;
	pos = present(after);
	fflush(stdout);
	if (pos)
	{
		specificRemove(pos + 1, result);
		return 1;
	}
	else
	{
		printf("ERROR REMOVING");
		return 0;
	}
}

// remove before takes the item pos - 1 to remove and places in result
int remove_before(const char *before, char *result)
{

	int pos;
	pos = present(before);
	fflush(stdout);
	if (pos)
	{
		specificRemove(pos-1, result);
		return 1;
	}
	else
	{
		printf("ERROR REMOVING");
		return 0;
	}
}

int main(int argc, char *argv[])
{
	insert_string("Alpha");
	insert_string("Bravo");
	insert_string("Charlie");
	insert_after("Bravo", "Delta");
	insert_before("Alpha", "Echo");
	print_list(); // Charlie -> Bravo -> Delta -> Echo -> Alpha

	char result[10];
	int ret;

	ret = remove_after("Delta", result);
	if (ret){
		printf("Removed: %s\n", result);
	}
	

	ret = remove_before("Bravo", result);

	if (ret){
		printf("Removed: %s\n", result);
	}

	ret = remove_string(result);

	
	if (ret){
		printf("Removed: %s\n", result);
	}


	print_list();
}
