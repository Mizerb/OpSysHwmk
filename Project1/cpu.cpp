/* cpu .cpp */ 

#include <string.h>
#include <stdio.h>

#include "cpu.h"

#define BUFF_LEN 255


void Result::print_me()
{
  // if I need this, it is here;
  return;
}

void Result::write_out(FILE* of , std::string name)
{
  if(task_count < 1) task_count = 1;

  fprintf(of, "Algorithm%s\n", name.c_str());

  double avg_burst = ((double)CPU_burst_time)/((double)task_count); //idk what this is...
  fprintf(of, "-- average CPU burst time: %.2f  ms\n", avg_burst );

  double avg_wait = ((double)CPU_burst_time)/((double)task_count); 
  fprintf(of, "-- average wait time: %.2f  ms\n", avg_wait );

  double avg_turn = ((double)CPU_burst_time)/((double)task_count); 
  fprintf(of, "-- average turnaround time: %.2f  ms\n", avg_turn );

  
  fprintf(of, "-- total number of context switches: %i\n", context_swaps );


  return;
}




Cpu::Cpu()
{
  time =0;
} 

Result Cpu::RUN()
{
  // internal things and safties!!
  context_countdown = 9;
  return execute_run();
}

void Cpu::queue_populate( FILE * fp)
{
  //const char r[] = "r";
  char buff[BUFF_LEN];
  Proc_Queue new_q(0);
  while( fgets(buff, BUFF_LEN , fp ))
  {
  	if( strstr(buff, "#") != NULL) continue;
  	Proc new_proc;
  	sscanf(buff, "%i|%i|%i|%i", 
  	    &(new_proc.proc_num) , 
  	    &(new_proc.inital_burst_time), 
  	    &(new_proc.num_burst) , 
  	    &(new_proc.inital_io_time)
  	    );
  	printf("NEW THING %i|%i|%i|%i\n", 
  		(new_proc.proc_num) , 
  	    (new_proc.inital_burst_time), 
  	    (new_proc.num_burst) , 
  	    (new_proc.inital_io_time));

  	new_q.add_proc(new_proc);
  }

  return;
}

bool Cpu::not_done()
{
  return proc_q.is_empty();
}

void Cpu::reset()
{
  time = 0;
  proc_q = inital_q;
}

void Cpu::change_type( int i)
{
  inital_q.change_type(i);
  proc_q.change_type(i);
  return;
}

void Cpu::IO_dealings()
{
  //increments all things in IO;
  for(std::list<Proc>::iterator it= io_now.begin(); it != io_now.end() ; ++it )
  {
  	it.io-- 
  }
}

void Cpu::execute_tick()
{
  time++;
  return;

}

Result Cpu::execute_run()
{
  Result Ret;
  while( not_done())
  {
    //If No Proc, grab proc
    if( context_countdown == 0)
    {
      // GRAB NEW TASK.
      context_countdown = -1;
      burst_now = proc_q.get_next();
      
      burst_now.


      printf("time %ums: P%i started using the CPU %s", time, 
        burst_now.proc_num , (proc_q.printQ()).c_str() ); // Truly evil I know.
    }
    else if( context_countdown < 0)
    {
      
    }
    



    proc_q.increment();
    //return Ret;
  }
  return Ret;
} 