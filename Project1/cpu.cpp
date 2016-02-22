/* cpu .cpp */ 

#include <string.h>
#include <stdio.h>

#include "cpu.h"

#define BUFF_LEN 255

#define PRINT_Q (proc_q.print_Q()).c_str()

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

Cpu::Cpu(int _core_count)
{
  temper core;
  std::list<Core> temp(_core_count, temper);
  cores = temp;
  time = 0;
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

void Cpu::increment_cores()
{
  for(std::list<Core>::iterator it= cores.begin(); it != cores.end() ; ++it )
  {
  	it->increment();
  }
  return;
}


void Cpu::cores_check_all()
{
  for(std::list<Core>::iterator it= cores.begin(); it != cores.end() ; ++it )
  {
    if(it->rdy_for_proc())
    {     
      it->is_context_swapping = false;
      
      it->receive_proc( proc_q.get_next() );

      printf("time %lums: P%i started using the CPU %s", time, 
        burst_now.proc_num , PRINT_Q ); // Truly evil I know.
    }
    else if(it->burst_now.burst_time == 0) //Get process out of CPU
    {
      it->start_context_swap();
      burst_end(it->burst_now);
    }
  }
}



void Cpu::burst_end( Proc dead_proc)
{
  dead_proc.num_burst--;
  if(dead_proc.inital_io_time > 0)
  {
    printf("time %lums: P%i completed its CPU burst %s\n", 
      time,dead_proc.proc_num, PRINT_Q);
    new_io( dead_proc);
    return;
  }
  if(dead_proc.num_burst > 0)
  {
    proc_q.add_proc(dead_proc);
  }
}


void Cpu::new_io(Proc new_proc)
{
  new_proc.io_time= new_proc.inital_io_time;
  io_now.push_back( new_proc );

  printf("time %lums: P%i blocked on I/O %s\n", time,
    new_proc.proc_num, PRINT_Q);
}

void Cpu::End_of_Proc(Proc dead_proc)
{
  //TO BE USED IN PROCESS TERMINATION;
  return;
}

void Cpu::IO_dealings()
{
  //increments all things in IO;
  for(std::list<Proc>::iterator it= io_now.begin(); it != io_now.end() ; ++it )
  {
  	if(it->io_time == 0 )
  	{
  	  printf("time %lums: P%i completed I\\O %s", time, it->proc_num, PRINT_Q);
  	}
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

  char name[] = "";
  printf("time 0ms: Simulator started for %s %s\n",
    name, PRINT_Q);
  time = -1; 
  while( not_done())
  {
    time++;
    increment_cores();
    increment_IO();
    proc_q.increment();
    
    //Everything is now up to time;
    // All we need to do is check that we're working


    
  

  }
  return Ret;
} 