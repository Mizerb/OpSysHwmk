// INCLUDES *****************************************************************************
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <list>

#include "cpu.h"

// DEFINES ******************************************************************************

#define file_name "processes.txt"

#define FCFS 0
#define SJF  1
// **************************************************************************************

void useage_error()
{
  std::cerr << " Hey, you screwed up" <<std::endl;
  exit(1);
}


int main( int argc , char* argv[])
{
  if( argc != 3)
  { 
    fprintf(stderr,"Hey, wrong inputs buddy\n");
    return 1;
  }
  return 0;
}



