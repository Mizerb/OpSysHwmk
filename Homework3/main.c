/* main.c BEN MIZERA */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_WORD_SIZE 10000  // Make smaller if you wish, had to avoid stack 
#define INITAL_LIST_SIZE 16  //    smashing. Was terrible. Had me stuck for 1/2 hour
#define MAX_FILE_COUNT 128

pthread_mutex_t index_mutex =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t alloc_mutex =PTHREAD_MUTEX_INITIALIZER;

void mem_error()
{
  fprintf( stderr, "THERE'S A MEMORY ISSUE\n");
  exit(1);
}

void print_usage()
{
  //printf( "USAGE:\n ARG_1: text file name (i.e. \"file.txt\")\nARG_2: Search String (i.e. \"meh\")\n " );
  perror("ERROR: Invalid arguments\nUSAGE: ./a.out <directory> <substring>\n");
  //why not fprintf(stderr, ...) ?
  return;
}

typedef struct c_word  /* why? Because I like to think about things this way */
{
  char* text;
  char* file_name;
}word;

typedef struct c_word_list
{
  word* list;
  int max_size;
  int current_size; 
}word_list;

struct pass_in
{
  char * file_name;
  word_list * the_list;
  char* path;
};

word_list * new_word_list()
{
  word_list * ret = (word_list*) malloc( sizeof(word_list) ); 
  if( ret == NULL ) mem_error();
  ret->list = (word*) calloc(INITAL_LIST_SIZE, sizeof(word) );
  //printf("%d\n", INITAL_LIST_SIZE);
  if( ret->list == NULL ) mem_error();
  ret->max_size = INITAL_LIST_SIZE;
  ret->current_size = 0;
  printf("Allocated initial array of %d character pointers.\n", ret->max_size);
  return ret;
}

/* For this function, I elected to use 2 mutexes, which seemed like what the
 * Homework document was suggesting. I'm not entirly sure, but here it is. 
 * All other operations (reading, copying, allocating) are done in parrellel,
 * leaving the list realloc and the index grabbing as tasks I needed to synronize.
 * I might have done the alloc_mutex section differently, but I didn't want to lock
 * a mutex in another mutex. So we're left with this, slight mess. 
 * I also thought about having the check size outside the mutex lock, however,
 * doing that would have caused all the threads to see the size issue and attempt
 * to do the realloc. So it kinda made sense to just lock that check for safty. 
 */

void append_word_list( word_list * list , word new_word)
{
  
  pthread_mutex_lock( &alloc_mutex );
  if(list->current_size == list->max_size)
  {
    list->list = (word*) realloc( list->list, list->max_size*2*sizeof(word) );
    if( list->list == NULL ) mem_error();
    list->max_size = list->max_size *2;
    printf( "THREAD %u: Re-allocated array of %d character pointers.\n", 
      (unsigned int)pthread_self(), list->max_size );
  }
  pthread_mutex_unlock( &alloc_mutex );
  
  //Grab index, safely 
  int index;
  pthread_mutex_lock( &index_mutex );
  index = list->current_size++;
  pthread_mutex_unlock( &index_mutex );

  list->list[index] = new_word;
  
  printf("THREAD %u: Added '%s' at index %i.\n", 
    (unsigned int)pthread_self(), new_word.text, index);

  //printf("%s\n", (list->list[list->current_size].text) );
  //list->current_size++;
#if 0
  printf( "current_size is %d\n" , list->current_size);
  printf( "max_size is %d\n" , list->max_size);
#endif

}

void list_free( word_list * list)
{
  int i;
  for( i = 0; i < list->current_size ; i++)
  {
   // fprintf( stderr, "Shit hit the fan here right?\n");
    free(list->list[i].text);
    free(list->list[i].file_name);
  }
  free(list->list);
  free(list);
}

int is_desired_file(char * dir, char *file_name)
{
  /*
  int len = strlen(file_name);
  return ((len > 4) && (strcmp(len + file_name - 4, ".txt") == 0));
  */
  char path[MAX_WORD_SIZE]; path[0] = 0;
  struct stat status;
  strcat(path, dir);
  if( strstr(dir, "/") == NULL ) strcat(path, "/");
  strcat(path, file_name);


  if(!stat(path,&status))
  {
    if(S_ISREG(status.st_mode))
    {
      //printf("ITS A FILE! PATH:%s , %s\n", path , file_name);
      return 1;
    }
  }
  //printf("Not a file? PATH %s , %s\n", path, file_name);
  return 0;

}

FILE * file_open(  char * input_file )
{
  FILE * fp;
  fp = fopen( input_file , "r" );
  if( fp == NULL )
  {
    fprintf( stderr , "THREAD %u: Can't open inputfile %s\n",
      (unsigned int)pthread_self(), input_file  );
    exit(1);
  } 
  return fp;
}

void * list_populate( void * arg )
{
  struct pass_in * args = (struct pass_in *)arg;
  
  FILE* fp;

  fp = file_open( args->path );
  free( args->path);

  char BUFFER[MAX_WORD_SIZE]; //This was causing stack Smashing
  for(;;)
  {
    //Read next word from File and check for End of File
    int result  = fscanf(fp, "%s", BUFFER);
    if( result == EOF ) break;

    // Copy string into the new Word, as well as the Filename
    word local_word;
    local_word.text = malloc( (sizeof(char)*strlen(BUFFER))+1 );
    strcpy(local_word.text , BUFFER );

    local_word.file_name = malloc((sizeof(char)*strlen(args->file_name))+1);
    strcpy(local_word.file_name, args->file_name);

    //Add Word to List
    append_word_list( args->the_list , local_word);
  }
  // Close file and free up unneeded variables
  fclose(fp);
  free(args->file_name);
  free(args);

  return NULL;
}

void list_search( word_list * current_list , char * compare)
{
  int i; 
  printf("MAIN THREAD: Words containing substring \"%s\" are:\n", 
    compare);
  for(i = 0 ; i< current_list->current_size ; i++)
  {
    if( strstr(current_list->list[i].text, compare) != NULL)
    {
      printf("MAIN THREAD: %s (from '%s')\n", 
        current_list->list[i].text, current_list->list[i].file_name);
    }
  }
  return;
}

struct pass_in* pass_prep( char* dir_name, char* file_name, word_list* list)
{
  struct pass_in * pass_me = malloc(sizeof(struct pass_in));
  pass_me->the_list = list;
  
  pass_me->file_name = malloc((sizeof(char)*strlen(file_name))+1);
  strcpy( pass_me->file_name , file_name);

  char path[MAX_WORD_SIZE]; path[0]=0;

  strcat(path, dir_name);
  if( strstr(dir_name, "/") == NULL ) strcat(path, "/");
  strcat(path, file_name);

  pass_me->path = malloc(sizeof(char)*strlen(path)+1);
  strcpy(pass_me->path , path);

  return pass_me;
}

int main(int argc, char  *argv[])
{
  if(argc != 3){
    print_usage();
    return 0;
  }
  DIR* d;
  struct dirent *dir;

  d= opendir(argv[1]);


  word_list * current_list = new_word_list();
  pthread_t tid[MAX_FILE_COUNT]; int n =0;


  if (d == NULL)
  {
    fprintf(stderr, "ERROR: Directory Failed to Open\n");
    return EXIT_FAILURE;
  }

  while ((dir = readdir(d)) != NULL)
  {
    if( !(is_desired_file(argv[1] , dir->d_name)) ) continue;
    
    struct pass_in * pass_me = pass_prep(argv[1], dir->d_name, current_list);

    pthread_create(&tid[n++], NULL , list_populate, pass_me);
    printf("MAIN THREAD: Assigned '%s' to child thread %u.\n",
      dir->d_name, (unsigned int)tid[n-1]);
  }
  closedir(d);
  

  int i =0;
  for( i = 0 ; i < n ; i++)
  {
    pthread_join(tid[i], NULL);
  }
  printf("MAIN THREAD: All done (successfully read %d words).\n", 
    current_list->current_size);

  list_search( current_list,  argv[2]);

  list_free( current_list);

  return EXIT_SUCCESS;
}
