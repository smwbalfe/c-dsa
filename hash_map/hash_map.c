#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** hash_map;  // this is where you should store your names

// variable to store the size of the hashmap
int size = 0;

//variable to check whether the hashmap has been allocated some space.
int allocated = 0;

int socc = 0 ;

int hash_function(const char *key) {

	
	int charTotal = 0;

	// Increment to char total the ascii value of every letter in the key
	for (size_t i = 0; i < strlen(key); i++){
		charTotal += (int) key[i];
	}

	// Return this charTotal value modded with the size to obtain the correct index position to use
	return charTotal % size;
}

void print_map()
{
	for (size_t i = 0; i < size; i++)
	{
		printf("item %d: %s \n", i, hash_map[i]);
	}
}

void resize_map(int new_size) {

	// if the map has not already been allocated
	if (!allocated){

		// set an area of memory dedicated to the new size for this map
		hash_map = calloc(sizeof(char*), new_size);

		// alter the size
		size = new_size;

		// allocated now therefore 1.
		allocated = 1;
	}
	else{


		
		int temp, index;
		char **new_map;

		// create space for the new map
		new_map = calloc(sizeof(char *), new_size);
		// holds previous size to know how many iterations to do from the previous map for rehashing
		temp = size;
		size = new_size;
		for (size_t i = 0; i < temp; i++)
		{
			// if the index at that location is NULL then dont add any data to the allocated space, which is defautl as NULL
			if (hash_map[i] != NULL){

				// otherwise calculate the new hash location in the larger map
				index = hash_function(hash_map[i]);

				// if there is a space then add it to the map
				if (new_map[index] != NULL){
					new_map[index] = hash_map[i];
				}
				// otherwise linear probe to obtain a new location that is free
				else{
					new_map[linear_probe(index, new_map)] = hash_map[i];
				}
				// null out the old hash map to prevent dangling pointers.
				*(hash_map + i) = NULL;
			}
		}
		// free the old allocated hash map
		free(hash_map);

		// set hashmap to be our newly allocated larger map.
		hash_map = new_map;
	}
}
int linear_probe(int index, char ** map)
{
	// while you arent at a free index
	while (map[index] != NULL){

		//increment index
		index++;

		// mod against size to loop back to the start
		index %=size;
	}

	// return corrected index
	return index;
}

// returns the load factor = occupied slots / total slots
double load_factor(){

	return (double) (socc / size);
}

int search_map(const char *name)
{

	
	int index;
	// hash the name 
	index = hash_function(name);

	// if the item at this location is not empty
	if (!(hash_map[index] == NULL)){
		// if the index isnt equal to that item
		if (!(hash_map[index] == name))
		{
			while(hash_map[index] != NULL){
				index++;
				index %= size;
				if (hash_map[index] == name){
					return index;
				}
			}

		}
		else
		{
			return index;
		}
	}

	// this hits if the linear probe couldnt find or if it hashed instantly to a NULL position meaning its at a new location.
	return 0;
}

void add_to_map(const char *name) {
	// if the item is not already present in the map then do not add.
	if (search_map(name) == 0 ){

		// otherwise find the index to insert into
		int index = hash_function(name);

		// if the indexed location is empty then just add the name to that position.
		if (hash_map[index] == NULL) {
			hash_map[index] = name;
		}
		else {
			// otherwise linar probe to a free location
			hash_map[linear_probe(index,hash_map)] = (char *) name;
		}
	
		// check new load factor and double the number of slots present in the map if so
		socc++;
		if (load_factor() > 0.7){

			// double the size of the map
			resize_map((size) * 2);
		}
	
	}
}

int remove_from_map(const char *name) {

	// find the index hashed
	int removeIndex = search_map(name);

	// search and find the index to remove
	if (removeIndex){

		// remove the item by setting it as NULL.
		hash_map[removeIndex] = NULL;

		// decrement the number of size occupied.
		socc--;
		return 1;
	}
	return 0;
}


int main(int argc, char *argv[]) {
	char *stringOne = "Hello World";
	char *stringTwo = "How are you?";
	char *stringThree = "Ae the best you...!!";
	char *stringFour = "Be kind to yourself";
	char *stringFive = "Principles of Programming 2";

	resize_map(6);
	add_to_map(stringOne);
	add_to_map(stringTwo);
	add_to_map(stringOne);
	add_to_map(stringThree);
	add_to_map(stringFour);
	add_to_map(stringFive);
	
	print_map();
	int ret = search_map(stringOne);
	if(ret)
	 	printf("Found %s!\n", stringOne);
	remove_from_map(stringFive);
	ret = search_map(stringFive);
	if(ret)
		printf("Found test %s!\n", stringFive);
	print_map();
}