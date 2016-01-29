/* main.c BEN MIZERA */
#include <stdio.h>
#include <stdlib.h>

#define MAX_WORD_SIZE 128
#define INITAL_LIST_SIZE 16

void mem_error()
{
	fprintf( stderr, "THERE'S A MEMORY FUCK UP\n");
	exit(1);
}

void print_usage()
{
  printf( "USAGE:\n ARG_1: text file name (i.e. \"file.txt\")\nARG_2: Search String (i.e. \"meh\") " );
  return;
}

#define word struct c_word 
struct c_word /* why? Because I like to think about things this way */
{
	char* text;
};

#define word_list struct c_word_list
struct c_word_list
{
	word* list;
	int max_size;
	int current_size; 
};

word_list * new_word_list()
{
  word_list * ret = (word_list*) malloc( sizeof(word_list) ); 
  if( ret == NULL ) mem_error();
  ret->list = (word*) calloc(INITAL_LIST_SIZE, sizeof(word) );
  printf("%d\n", INITAL_LIST_SIZE);
  if( ret->list == NULL ) mem_error();
  ret->max_size = INITAL_LIST_SIZE;
  ret->current_size = 0;
  printf("Allocated initial array of %d character pointers.\n", ret->max_size);
  return ret;
}

void append_word_list( word_list * list , word new_word)
{
  list->list[list->current_size++] = new_word;
#if 0
  printf( "current_size is %d\n" , list->current_size);
  printf( "max_size is %d\n" , list->max_size);
#endif
  if(list->current_size == list->max_size)
  {
    list->list = (word*) realloc( list->list, list->max_size*2*sizeof(word) );
    if( list->list == NULL ) mem_error();
    list->max_size = list->max_size *2;
    printf( "Re-allocated array of %d character pointers.\n", list->max_size );
  }
}

void list_free( word_list * list)
{
	int i;
	for( i = 0; i < list->current_size ; i++)
	{
		fprintf( stderr, "Shit hit the fan here right?\n");
		free(list->list[i].text);
	}
	free(list->list);
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

word_list * list_populate( FILE * fp )
{
  word_list * current_list = new_word_list();
  for(;;)
  {
    word local_word;
	local_word.text = malloc( sizeof(char)*9 );
    int result  = fscanf(fp, "%s", local_word.text);
	if( result == EOF )
	{
		free(local_word.text);
		break;
	} 
    printf( "%s\n" , local_word.text);
    append_word_list( current_list , local_word);
  }
  fclose(fp);
  printf("All done (successfully read %d words).\n", current_list->current_size);

  return 0;
}

void list_search( word_list * current_list)
{
  return;
}

int main(int argc, char  *argv[])
{
  if(argc != 3){
    print_usage();
    return 0;
  }
  FILE* fp; 

  fp = file_open( argv[1] );

  word_list * current_list = list_populate(fp);
  
  list_search( current_list);

  //list_free( current_list);

  return 0;
}