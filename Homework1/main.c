/* main.c BEN MIZERA */
#include <stdio.h>
#include <stdlib.h>

#define MAX_WORD_SIZE 128

#define word struct c_word 
struct c_word /* why? Because I like to think about things this way */
{
	char* text;
};

#define word_list struct c_word_list
struct c_word_list
{
	word* list;
	unsigned max_size;
	unsigned current_size; 
};

void append_word_list( word_list * list , word new_word)
{
	list->list[list->current_size++] = new_word;

	if(word_list_index == word_list_size)
	{
		word_list = (word*) realloc( word_list, word_list_size*2*sizeof(word) );
		word_list_size = word_list_size *2;
		printf( "Re-allocated array of %d character pointers.\n", word_list_size );
	}
}

void print_usage()
{
	printf( "USAGE:\n ARG_1: text file name (i.e. \"file.txt\")\nARG_2: Search String (i.e. \"meh\") " );
	return;
}

FILE * file_open(  char * input_file )
{
  FILE * fp;
  fp = fopen( input_file , "r" );
  if( fp == NULL )
  {
    fprintf( stderr , "Can't open inputfile\n" );
    exit(1);
  }
}

word * list_populate( FILE * fp )
{
	char a; 
	unsigned int word_list_size = 16; 
	unsigned int word_list_index = 0;
	word * local_word;

	word word_list[] = calloc( 16 , sizeof(word) );
	printf("Allocated initial array of 16 character pointers.");
	do
	{
		a = fgetc( fp );
		if( isspace(a) )
		{
			word_list[word_list_index++] = local_word;

			if(word_list_index == word_list_size)
			{
				word_list = (word*) realloc( word_list, word_list_size*2*sizeof(word) );
				word_list_size = word_list_size *2;
				printf( "Re-allocated array of %d character pointers.\n", word_list_size );
			}
			local_word = malloc( sizeof(word) );
			local_word -> n = 0;
			local_word -> text = calloc( MAX_WORD_SIZE , sizeof(char) );
		}
		else if( /*normal char*/ )
		{
			local_word -> text[local_word->n++] = a;
		}

	}while( a != EOF);
}

int main(int argc, char  *argv[])
{
	if(argc != 3){
		print_usage();
		return 0;
	}
    FILE* fp; 

    fp = file_open( argv[1] );

    word * word_list = list_populate(fp);

	return 0;
}