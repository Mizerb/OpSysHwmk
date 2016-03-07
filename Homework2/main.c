#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_SIZE 255

typedef struct c_expression
{
  char operation;
  int result;
  
  struct c_expression * arguments;
  char * raw; 
  
  int ** upsend_pipes; 
  
  
  int * return_pipe;
}exp;

void set_exp( exp *mine)
{
	mine->arguments = NULL;
    mine->raw = NULL;
    mine->upsend_pipes = NULL;
    mine->return_pipe = NULL;
}

exp parse_exp( char * str )
{
    exp ret; //= malloc(sizeof(exp));
    set_exp(&ret);

    printf( "New exp:  %s\n" , str);

    char * tok;

    tok = strtok(str ," ");
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
    else if(strstr(tok , "+")!= NULL )
    {
        ret.operation = '-';
    }
    else
    {
        sscanf(str, "%d" , &(ret.result));
        return ret;
    }
    
    tok = strtok( str , " ");
    int i =0;
    while(tok != NULL )
    {
        ret.arguments[i++] = parse_exp(tok);
    }
    return ret;
}

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
    
    exp ret = parse_exp(BUFFER);
    

    
    return 0;
}
