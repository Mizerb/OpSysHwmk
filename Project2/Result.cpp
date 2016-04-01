#include "cpu.h"
#include <stdio.h>

void Result::print_me()
{
  // if I need this, it is here;
  return;
}

void Result::add_proc( Proc dead_proc)
{
  CPU_burst_time+= (dead_proc.inital_num_burst * dead_proc.inital_burst_time);
  total_wait_time+= dead_proc.wait_time;
  total_turn_time+= dead_proc.turn_time;
  task_count++;
  
  //printf("TASK_COUNT: %i\n", CPU_burst_time);
}

void Result::write_out(FILE* of , std::string name)
{
  fprintf(of, "Algorithm %s\n", name.c_str());

  double avg_burst = ((double)CPU_burst_time)/((double)context_swaps); //idk what this is...
  fprintf(of, "-- average CPU burst time: %.2f ms\n", avg_burst );

  double avg_wait = ((double)(total_wait_time-task_count))/((double)context_swaps); 
  fprintf(of, "-- average wait time: %.2f ms\n", avg_wait );

  double avg_turn = ((double)total_turn_time-task_count)/((double)context_swaps); 
  fprintf(of, "-- average turnaround time: %.2f ms\n", avg_turn );

  
  fprintf(of, "-- total number of context switches: %i\n", context_swaps );


  return;
}