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
	unsigned max_size;
	unsigned current_size; 
};

word_list * new_word_list()
{
  word_list * ret = (word_list*) malloc( sizeof(word_list)); 
  if( ret == NULL ) mem_error();
  ret->list = (word*) calloc(INITAL_LIST_SIZE, sizeof(word) );
  if( ret->list == NULL ) mem_error();
  ret->max_size = INITAL_LIST_SIZE;
  ret->current_size = 0;
  printf("Allocated initial array of 16 character pointers.");
  return ret;
}

void append_word_list( word_list * list , word new_word)
{
  list->list[list->current_size++] = new_word;

  if(list->current_size == list->max_size)
  {
    list->list = (word*) realloc( list, list->max_size*2*sizeof(word) );
    if( list->list == NULL ) mem_error();
    list->max_size = list->max_size *2;
    printf( "Re-allocated array of %d character pointers.\n", list->max_size );
  }
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
  return 0;
}
/*
int Name_Grab( char* input_file, m_string* input_names , int n)
{
  FILE * fp;
  fp = fopen(input_file , "r");
  if( fp == NULL)
  {
    fprintf(stderr , "Can't open inputfile\n");
    exit(1);
  }
  int i;
  for(i =0 ; i<n ; i++)
  {
    m_string local;
	char * name = malloc( sizeof(char)*9);
    fscanf(fp, "%s", name);
	local.name = name;
    //printf( "%s\n" , local.name);
    input_names[i] = local;
  }

  return 0;
}
*/

int main(int argc, char  *argv[])
{
  if(argc != 3){
    print_usage();
    return 0;
  }
  FILE* fp; 

  fp = file_open( argv[1] );

  word_list * current_list = list_populate(fp);

  return 0;
}