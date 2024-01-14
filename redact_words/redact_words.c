/*
 Implement an algorithm in C which given a file containing a block of text as
 input, redacts all words from a given set of “redactable” words (also from a
 file), and outputs the result to a file called “result.txt”. For example,
 given the block of text:
    The quick brown fox jumps over the lazy dog

 and the redactable set of words:
    the, jumps, lazy

 the output text in “result.txt” should be
    *** quick brown fox ***** over *** **** dog

*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>


#define MATCH_NOT_FOUND SIZE_MAX

size_t findstring(char *str, const char *match, size_t current_pos, int ignore_case);

size_t fsize(FILE * fp){
	fseek(fp, 0, SEEK_END);
	size_t fileLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return fileLength;
}

char *strcpy(char *destination, const char *source)
{
	char *pointer = destination;

	while (!(*source == '\0' && *pointer == '\0')){
		*pointer = *source;
		pointer++;
		source++;
	}

	return pointer;
}

size_t strlenx(const char *s)
{
	size_t n = 0;
	while (s[n]) ++n;
	return n;
}

size_t findstring(char *str, const char * match, size_t pos, int ignore_case)
{

	// initialise the match position
	size_t match_pos = 0;
	// calculate the length of the phrase trying to match
	size_t match_len = strlenx(match);
	// this returns the start index intialised as a large number
	size_t start_match = MATCH_NOT_FOUND;

	// iterate over the string from the position located by pos
	while (str[pos])
	{
		// finding a matching character
		if ((str[pos] == match[match_pos]))
		{
			// In a potential match:

			// set the start match to be this position that was found
			if (start_match == MATCH_NOT_FOUND)
				start_match = pos;
			// increment the match
			match_pos++;
		}
		else
		{
			// the character was wrong therefore reset the start index and the match pos that is present
			start_match = MATCH_NOT_FOUND;
			match_pos = 0;
		}
		
		// if match_pos is now equal to the length of the phrase, the phrase is complete and located, thus break out of the loop
		// and return where the phrase was found
		if (match_pos == match_len)
		{
			// Found full match, we're done
			break;
		}

		// move the character along
		pos++;
	}
	return start_match;
}
void *memsetx(void *s, int c, size_t len){
	// set the destination pointer to be the area in memory we would like to set the values of
	char *dst = s;

	// len is the number of bytes we wish to offset from the pointer of dst to fill in the character c in that location
	// so loop until it reaches 0
	while (len > 0){
		// at each iteration dereference the pointer and place the character

		// when redacting I wish to memset the area but not remove the spaces that are between multi word phrases.
		if (!isspace(*dst)){
			*dst = c;
		}
		// move on to the next pointer to place the next character
		dst++;
		// decrement length
		len--;
	}
	// return the original memory location as a void *
	return s;
}

// search for the first occurence of character c within the given char s
const char *strchrx(const char *s, int c)
{
	// while the string is not at the null terminator, compare to the character passed in
	do
	{
		// if the character at the current pointer location is equal to the character
		if (*s == c)
		{
			// return the offsetted position in the char which corresponds to the correct index of the char in question
			return s;
		}
	
	} while (*s++);
}

unsigned int my_strcspn(const char *s1, const char *s2)
{
	unsigned int len = 0;
	// while the string being searched is not at its null terminator
	while (*s1)
	{
		// if you can locate the character being pointed at (*s1) within the string s2
		// return how much lenghth was incremented by
		if (strchrx(s2, *s1))
		{
			return len;
		}
		else
		{
			//increment s1
			s1++;
			//increment len variable
			len++;
		}
	}
	// length is just the number of characters that appear in s1 which are not in s2.
	return len;
}

void redact_words(const char *text_filename, const char *redact_words_filename){

	int count, position;
	const char** redact_words;
	char *redactFileBuffer, *line , *word;


	count = position = 0;
	FILE *redactWords, *redactFile, *resultFile;
	// open files for the correct respective modes of read/write
	redactWords = fopen(redact_words_filename, "r" );
	redactFile = fopen(text_filename, "r");
	resultFile = fopen("./result.txt", "w");

	// check to see that the files are not empty
	if (redactWords == NULL || redactFile == NULL){
		return;
	}

	// calculate the size of redact file to know how much space to allocate for its reading into a string.
	size_t fileLength = fsize(redactFile);
	// allocate space for the file to be read into
	redactFileBuffer = calloc(fileLength, sizeof(redactFileBuffer));
	fread(redactFileBuffer, fileLength, 1, redactFile);
	
	// create a buffer to hold the lines of the file
	line = calloc(1024, sizeof(line));
	
	// set the pointer to the start again
	fseek(redactWords, 0, SEEK_SET);
	// allocate space to store the redacted

	redact_words = calloc(8, sizeof(redact_words));
	position = 0 ;

	//word = calloc(1024, sizeof(word));
	while(fgets(line, 1024, redactWords) != NULL){;
		
		// makes sure its read line by line by setting a null terminator at each new line character.
		line[my_strcspn(line, "\n")] = 0;

		// allocate and move the line into the word
		char *wordMain = (char *)malloc(sizeof(char) * strlenx(line));
		strcpy(wordMain, line);
		// add to the array of redact words at the correct position
		*(redact_words+count) = wordMain;
		free(wordMain);

		// count the number of lines, also using this value to offset the array
		count++;
	}
	free(line);

	for (int i = 0; i < count; i++){

		const char * selectWord = redact_words[i];
		size_t found_pos = MATCH_NOT_FOUND;

		// this loop will scan the entire buffer for the sequence of characters held in selectWord and return where it found the first index
		// when its found MATCH_NOT_FOUND is returned
		while ((found_pos = findstring(redactFileBuffer, selectWord, found_pos+1, 0)) != MATCH_NOT_FOUND)
		{

			
			// update the original buffer updating the stars in the exact specific positions that were found by the string.
			memsetx(redactFileBuffer + found_pos, '*', strlenx(selectWord));
			
	
		}
	}

	// redactFileBuffer[fileLength] = '\0';
	printf("%s\n", redactFileBuffer);

	// write the buffer to output file
	fprintf(resultFile, "%s\n", redactFileBuffer);

	free(redactFileBuffer);
	free(redact_words);

	
	//close the output streams.
	fclose(redactWords);
	fclose(redactFile);
}

int main(int argc, char *argv[]) {
	const char *input_file = "./debate.txt";
	const char *redact_file = "./redact.txt";
	redact_words(input_file, redact_file);
	return EXIT_SUCCESS;
}
