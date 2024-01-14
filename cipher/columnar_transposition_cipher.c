/*
 ============================================================================
 Name        : CWK2Q7.c
 Author      : Anonymous (DO NOT CHANGE)
 Description :

 Implement a Columnar Transposition Cipher in C to encrypt a message of any
 length. A Columnar Transposition Cipher is transposition cipher that follows
 a simple rule for mixing up the characters in the plaintext to form the
 ciphertext.

 As an example, to encrypt the message ATTACKATDAWN with the keyword KEYS,
 we first write out our message as shown below,
    K	E	Y	S
    A	T	T	A
    C	K	A	T
    D	A	W	N

 Note: if the message to encode does not fit into the grid, you should pad
 the message with x's or random characters for example, ATTACKNOW with the
 keyword KEYS might look like below,
    K	E	Y	S
    A	T	T	A
    C	K	N	O
    W	X	X	X

 Once you have constructed your table, the columns are now reordered such
 that the letters in the keyword are ordered alphabetically,
    E	K	S	Y
    T	A	A	T
    K	C	T	A
    A	D	N	W

 The ciphertext is now read off along the columns, so in our example above,
 the ciphertext is TAATKCTAADNW.

 You should demonstrate your implementation by encrypting the file in the
 folder Q7 using the keyword - LOVELACE.

 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t fsize(FILE *fp)
{
   // set the pointer to the end of the file
   fseek(fp, 0, SEEK_END);

   // read the pointer to obtain to size of the file in bytes.
   size_t fileLength = ftell(fp);

   // return to the original pointer location
   fseek(fp, 0, SEEK_SET);

   // retunr the length of the filelength which is the exact number of bytes of the file. 
   return fileLength;

}

// function used to remove the gaps in the string in order to count the correct number of rows required in the
// column cipher.
void remove_spaces(char *s)
{

   // set set pointer to the start of the string.
   const char *d = s;
   do
   {
      // for any gap in the string that has been located
      while (*d == ' ')
      {
         //increment the pointer to skip
         ++d;
      }
      
      //set the original string to equal the destination which skips the original spaces
   } while (*s++ = *d++);
}

// used to sort the array index to match the values in an alphabetical order
// sorts ints as I passed in integers instead.
// https://stackoverflow.com/questions/24586499/keeping-track-of-the-original-indices-of-an-array-after-sorting-in-c
int compare(const void *pa, const void *pb)
{
   const int *a = pa;
   const int *b = pb;
   if (a[0] == b[0])
      return a[1] - b[1];
   else
      return a[0] - b[0];
}



void encrypt_columnar(const char *message_filename, const char *key, char **result){
   
   char * messageText, **grid, **index;
   int row,col, count, i , k;
   FILE *message;
   
   // open file and check if empty
   message = fopen(message_filename, "r");
   if (message == NULL)
   {
      return;
   }
   
   //obtain filelength to know how much space to allocate for the char array.
   size_t fileLength = fsize(message);
   messageText = malloc(sizeof(char) * (fileLength));

   // copy the file into the buffer in one movement.
   fread(messageText, fileLength, 1, message);
   messageText[fileLength] = 0;

   // spaces are to be ignored for encryption 
   remove_spaces(messageText);

   // column length is just the length of the key
   col = strlen(key);

   // row calculated by the length of the message to encrypt over the number of columns
   row = strlen(messageText) / col;

   // if the message is not exactly divisible into columns then increment the number of rows to fit
   if (strlen(messageText) % col)
   {
      row++;
   }

   // allocate the grid to add the characters too
   grid = malloc((row) * sizeof(char*));
   for(i = 0; i < row; i++){
      grid[i] = malloc(col * sizeof(char));
    
   }
 
   // an index that uses the key to sort alphabetically to build a list of index positions
   // which will be swapping rows about to follow the cipher
   int indexArr[col][2];
   for (int k = 0; k < strlen(key); k++)
   {
      indexArr[k][0] = (int) key[k];
      indexArr[k][1] = k;
   }
  
   // quick sort based only on the key column
   qsort(indexArr, col, sizeof indexArr[0], compare);

   // allocate space for the cipher
   char *cipher = calloc((row * col) , sizeof(char));

   // null terminate the message
   messageText[strlen(messageText)] = 0;

   // read the message into the grid , adding x if the indexed location is greater than the length of the message.
   for (int i = 0; i < row; i++){
      for (int k = 0; k < col; k++){
         // k + (col * i) is taking the current column but adding however larger the key was times i
         // to move the pointer along to fetch and add the correct characters
         if (k + (col * i) >= strlen(messageText)){
            grid[i][indexArr[k][1]] = 'x';
         }
         else{
            grid[i][indexArr[k][1]] = messageText[(k + (col * i))];
         }
         
         // null terminate each row
         grid[i][col] = 0;
      }
     
      // copy each completed row into the cipher at the correct offsetted location
      strncpy(cipher + (col * i), grid[i], col);
   }
 
   // null terminate the cipher
   cipher[col*row] = 0;

   // return as the result.
   *result = cipher;
}
int main(int argc, char *argv[]) {
	const char *example_message = "./text.txt";
	const char *example_key = "LOVELACE";
	char *encrypted_message = NULL;
	encrypt_columnar(example_message, example_key, &encrypted_message);
   printf("Encrypted message = %s \n", encrypted_message);
	
	return EXIT_SUCCESS;
}
