#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_SIZE 255

char * unused;

typedef struct c_expression
{
  char operation;
  int result;
  
  struct c_expression * arguments;
  int arg_count;
  char * raw; 
  
  int ** upsend_pipes; 
  
  
  int * return_pipe;
}exp;

void set_exp( exp *mine)
{
	
    mine->arguments = (exp*)malloc(20*sizeof(exp));
    mine->arg_count = 0;
    mine->raw = (char*)malloc(MAX_SIZE*sizeof(char));
    mine->upsend_pipes = NULL;
    mine->return_pipe = NULL;
}

char * find_end( char* start)
{
    int i = 0; int count = 0 ;
    for( i = 0 ; i< MAX_SIZE ; i++)
    {
        if(start[i] == ')' && count == 0) return start + i;
        if(start[i] == '(') count++;
        if(start[i] == ')') count--;
    }
    fprintf(stderr, "Unable to find ending\n");
    exit(1);
}

exp int_exp( int a)
{
    exp ret; set_exp( &ret);
    ret.result = a;
}

exp parse_exp( char * str , char *start , char * end)
{
    exp ret;
    set_exp( ret);

    if( *start == '+' ) ret.operation = '+';
    if( *start == '*' ) ret.operation = '+';
    if( *start == '/' ) ret.operation = '+';
    if( *start == '-' ) ret.operation = '+';
    else
    {
        char * tok  = strtok( start , " " );
        fprintf(stderr , "PROCESS %d: ERROR: unknown \"%s\" operator\n", getpid(), tok);
        exit(1);
    }
    
    start++;
    for(; start != end ; start++)
    {
        
    }
    
}

/*
exp parse_exp( char * str , char * unusedz)
{
    exp ret; //= malloc(sizeof(exp));
    set_exp(&ret);

    printf( "New exp:  %s\n" , str);
    char * tok;

    tok = str;// = strtok_r(NULL ," " , &unused);
    
    printf("tok = %s\n", tok);
    
    if( tok == NULL )
    {
        fprintf(stderr, "FUCK FUCK FUCK FUCK FUFKC \n");
        //WHo needs descriptive errors?
    }
    if( strstr(tok , "+") != NULL)
    {
        ret.operation = '+';
    }
    else if(strstr(tok , "-")!= NULL )
    {
        ret.operation = '-';
    }
    else if(strstr(tok , "*")!= NULL )
    {
        ret.operation = '*';
    }
    else if(strstr(tok , "/")!= NULL )
    {
        ret.operation = '/';
    }
    else
    {
        ret.operation = '?';
        sscanf(str, "%d" , &(ret.result));
        
        return ret;
    }
    
    tok = strtok_r( NULL , " " , &unused);
 
    while(tok != NULL )
    {
        ret.arguments[ret.arg_count++] = parse_exp(tok, unused);
        
        printf( "%c -> %d\n" , ret.operation,ret.arguments[ret.arg_count-1].result );
        if(strstr(tok , ")")!= NULL)break;
        tok = strtok_r( NULL , " " , &unused);
    }
    if( tok == NULL) printf("NULLED: %c\n" , ret.operation);
    return ret;
}
*/

int main(int argc, char* argv[])
{
    char BUFFER[MAX_SIZE];
    
    FILE * fp = fopen( argv[1] , "r");
    if( fp == NULL)
    {
        fprintf(stderr , "Incorrect args or file doesn't exist\n");
        exit(1);
    }
    
    while( fgets(BUFFER, MAX_SIZE, fp))
    {
        if( strstr(BUFFER,"#") != NULL) continue;
    }

    
    char * stuff;
    char * tok = strtok_r( BUFFER , " " , &unused);
    
    exp ret = parse_exp(BUFFER+1,stuff);
    
    
    
    
    return 0;
}
