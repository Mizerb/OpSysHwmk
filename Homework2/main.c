#include <stdio.h>
#include <stdlib.h>

struct expression
{
  char operation;
  unsigned result;
  expression * arguments;
};


int main(int argc, char* argv[])
{
  printf("I hate to think on how I'm going to parse a stupid text thing\n");
}
