#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************
GLOBALS **********************************************************************
*****************************************************************************/

unsigned int g_global_time;
unsigned int g_context_swap_count;


// Whatever....

typedef Proc
{
  int proc_num;
  int burst_time;
  int num_burst;
  int io_time;
  
  int wait_time;
  int turn_around_time;
} 
