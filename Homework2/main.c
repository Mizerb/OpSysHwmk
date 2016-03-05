#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct expression
{
  char operation;
  unsigned result;
  expression * arguments;
};


int main(int argc, char* argv[])
{
    FILE * fp = fopen( argv[1] , "r");
    if( fp == NULL)
    {
        fprintf(stderr , "Incorrect args or file doesn't exist\n");
        exit(1);
    }
    
    return 0;
}
