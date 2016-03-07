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
  
  int * upsend_pipes; 
  
  
  int return_pipe;
}exp;

void set_exp( exp *mine)
{
	
    mine->arguments = (exp*)malloc(20*sizeof(exp));
    mine->arg_count = 0;
    mine->raw = (char*)malloc(MAX_SIZE*sizeof(char));
    mine->upsend_pipes = (int*)malloc(10*sizeof(int));
    mine->return_pipe = 0;
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
    ret.operation = '?';
    return ret;
}

exp parse_exp( char * str , char *start , char * end)
{
    exp ret;
    set_exp( &ret);
    
    strncpy(ret.raw ,  start , end - start);
    //printf("%s\n" , ret.raw);
    
    printf( "STARTING: %c\n" , *start);
    if( *start == '+' ) ret.operation = '+';
    else if( *start == '*' ) ret.operation = '*';
    else if( *start == '/' ) ret.operation = '/';
    else if( *start == '-' ) ret.operation = '-';
    else
    {
        char * tok  = strtok( start , " " );
        fprintf(stderr , 
          "PROCESS %d: ERROR: unknown \"%s\" operator\n",
           getpid(), tok);
        exit(1);
    }
    
    start++;
    for(; start <end ; start++)
    {
        if( *start == '(' )
        {
            char* a_end = find_end( start +1 );
            
            ret.arguments[ret.arg_count++]=
              parse_exp( str, start +1 , a_end);
            start = a_end +1;
        }
        else if( *start == ' ') continue;
        else if( (*start >= '0' && *start <= '9') || *start=='-' )
        {
            int temp = atoi( start);
            while( *start != ' ' && *start != ')') start++;
            //printf( "FOUND %d\n" , temp);
            exp to_add = int_exp(temp);
            //printf("%c -> %d\n", ret.operation , to_add.result);
            ret.arguments[ret.arg_count++]= to_add;
            
            
            printf( "%c -> %d\n" , 
              ret.operation,
              ret.arguments[ret.arg_count-1].result 
              );
        }
    }
    return ret;
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
int calculate( int base , int to_add , char op)
{
    if(op=='+') return base + to_add;
    if(op=='-') return base - to_add;
    if(op=='/') return base / to_add;
    if(op=='*') return base * to_add;
    if(op=='?')
    {
        fprintf(stderr, "This shouldn't be here\n");
        exit(1);
    }
    else
    {
        fprintf(stderr, "What the hell %c\n" , op);
        exit(1);
    }
}

void pipe_check( int rc)
{
    if( rc == -1)
    {
        fprintf(stderr,"Pipe open fked up\n");
        exit(1);
    }
}

void fork_check( int pid)
{
    if( pid == -1)
    {
        fprintf(stderr,"Fork open fked up\n");
        exit(1);
    }
}

void exec( exp  mine)
{
    if( mine.operation == '?')
    {
        printf("PROCESS %d: Sending \"%d\" on pipe to parent\n",
          getpid(), mine.result );
        write( mine.return_pipe, &(mine.result), sizeof(int));
        close( mine.return_pipe);
        exit(0);
    }
    
    printf("PROCESS %d: Starting \"%c\" operation\n",
      getpid(), mine.operation);
    
    int i = 0 , rc;
    for( i = 0 ; i < mine.arg_count ; i++ )
    {
        int * pipy = (int *) malloc( sizeof(int) * 2);
        rc = pipe( pipy);
        pipe_check( rc);
        
        int pid = fork();
        fork_check(pid);
        
        if( pid == 0) // CHILD
        {
            close(pipy[0]);// close read end;
            exp child = mine.arguments[i];
            child.return_pipe = pipy[1];
            exec(child);
        }
        else          //Parent
        {
            close(pipy[1]);// close write end;
            mine.upsend_pipes[i] = pipy[0];
            
        }
        
    }
    mine.result = 0;
    for( i = 0 ; i< mine.arg_count ; i++ )
    {
        int temp;
        int bytes = read(mine.upsend_pipes[i],&temp,sizeof(int) );
        if( i == 0 ) mine.result = temp;
        else mine.result = calculate( mine.result , temp, mine.operation);
        close(mine.upsend_pipes[i]);
    }
    
    if(mine.return_pipe == 0)
    {
        printf("PROCESS %d: Processed \"(%s)\"; final answer is \"%d\"\n",
          getpid(),
          mine.raw,
          mine.result);
    }
    else
    {
        printf("PROCESS %d: Processed \"(%s)\"; sending \"%d\" on pipe to parent\n",
          getpid(),
          mine.raw,
          mine.result);
        write( mine.return_pipe, &(mine.result), sizeof(int));
        close( mine.return_pipe);
        exit(0);
    }
    
    return;
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
    
    exp ret = parse_exp(BUFFER,BUFFER+1, find_end(BUFFER+1) );
    
    exec(ret);
    
    
    
    return 0;
}
