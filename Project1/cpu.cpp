/* cpu .cpp */ 

#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "cpu.h"

#define BUFF_LEN 255

#define PRINT_Q (proc_q.print_Q()).c_str()

void Result::print_me()
{
  // if I need this, it is here;
  return;
}

void Result::add_proc( Proc dead_proc)
{
  CPU_burst_time+= (dead_proc.num_burst * dead_proc.inital_burst_time);
  total_wait_time+= dead_proc.wait_time;
  total_turn_time+= dead_proc.turn_time;
  task_count++;

  //printf("TASK_COUNT: %i\n", task_count);
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

void Core::increment()
{
  if(is_context_swapping)
  {
  	context_countdown--;
  	burst_now.is_context_swap();
  } 
  else burst_now.in_cpu_incre();
}

void Core::receive_proc( Proc new_proc)
{
  burst_now = new_proc;
  is_context_swapping = false;
  has_proc = true;
}

bool Core::rdy_for_proc()
{
  return (!has_proc && !is_context_swapping);
}

bool Core::rdy_to_start()
{
  return (is_context_swapping && (context_countdown==0));
}

void Core::start_context_swap()
{
  is_context_swapping = true;
  context_countdown = t_cs;
  has_proc = false;
  return;
}

void Core::wait_for_proc()
{
  is_context_swapping = false;
  has_proc = false;
}


Cpu::Cpu()
{
  time =0;
} 

Cpu::Cpu(int _core_count)
{
  Core temper;
  std::list<Core> temp(_core_count, temper);
  cores = temp;
  time = 0;
}


Result Cpu::RUN()
{
  // internal things and safties!!
  // Idk if there are any....
  printf("time 0ms: Simulator started for %s %s\n",
    (proc_q.print_type()).c_str(), PRINT_Q);

  Result Ret = execute_run();

  printf("time %lums: Simulator ended for %s\n\n\n",
    time, (proc_q.print_type()).c_str());

  return Ret;
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
/*
  	printf("NEW THING %i|%i|%i|%i\n", 
  		(new_proc.proc_num) , 
  	    (new_proc.inital_burst_time), 
  	    (new_proc.num_burst) , 
  	    (new_proc.inital_io_time));
  	    new_q.print_Q();
*/
  	new_q.add_proc(new_proc);
  }
  inital_q = new_q;
  return;
}

bool Cpu::not_done()
{
  if(Run_result.task_count == inital_q.get_size() )
  {
  	//printf( "Dumb shit\n");
  	return false;
  }
/*
  printf("TASK_COUNT = %i \t inital_q= %i \n" ,
  	Run_result.task_count, inital_q.get_size());
  printf("Dickbut\n");
*/
  return true;
}

void Cpu::reset()
{
  time = 0;
  proc_q = inital_q;
  Result meh;
  Run_result = meh;
  Core temper;
  std::list<Core> temp(cores.size(), temper);
  cores = temp;
  time = 0;
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

/*
void Cpu::cores_check_all()
{
  for(std::list<Core>::iterator it= cores.begin(); it != cores.end() ; ++it )
  {
    if(it->rdy_for_proc())
    { 
      if(proc_q.is_empty()) continue;

      it->receive_proc( proc_q.get_next() );

      printf("time %lums: P%i started using the CPU %s\n", time, 
        it->burst_now.proc_num , PRINT_Q ); // Truly evil I know.
      Run_result.context_swaps++;
    }
    else if(it->burst_now.burst_time == 0) //Get process out of CPU
    {
      it->start_context_swap();
      burst_end(it->burst_now);
    }
  }
}
*/

void Cpu::cores_check_all()
{
  for(std::list<Core>::iterator it= cores.begin(); it != cores.end() ; ++it )
  {
  	if(it->rdy_for_proc()) //Doesn't have a Proc
  	{
  	  if(proc_q.is_empty()) continue;
      
      it->start_context_swap();

      Run_result.context_swaps++;
  	}
  	else if(it->rdy_to_start())
  	{
      
      it->receive_proc( proc_q.get_next() );
      
      printf("time %lums: P%i started using the CPU %s\n", time, 
        it->burst_now.proc_num , PRINT_Q ); // Truly evil I know.
  	}
    else if(it->burst_now.burst_time == 0 && it->has_proc) //Get process out of CPU
    {
      it->wait_for_proc();
      burst_end(it->burst_now);
    }
  }
}




void Cpu::burst_end( Proc dead_proc)
{
  dead_proc.num_burst--;
  if(dead_proc.inital_io_time > 0 && dead_proc.num_burst > 0)
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
  else
  {
  	End_of_Proc(dead_proc);
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
  assert(dead_proc.num_burst <= 0);
  //TO BE USED IN PROCESS TERMINATION;
  printf("time %lums: P%i terminated %s\n", time, 
  	dead_proc.proc_num, PRINT_Q);
  
  //printf("INITAL COUNT = %i\n", inital_q.get_size());
  Run_result.add_proc( dead_proc);
  return;
}

void Cpu::increment_IO()
{
  for(std::list<Proc>::iterator it= io_now.begin(); it != io_now.end() ; ++it )
  {
    it->in_io_incre();
  }
}

void Cpu::end_of_IO(Proc dieing_proc)
{
  if( dieing_proc.num_burst == 0 && dieing_proc.inital_io_time > 0)
  {
  	proc_q.add_proc( dieing_proc);
  	printf("time %lums: P%i completed I/O %s\n", time, dieing_proc.proc_num, 
  	    PRINT_Q);
  }
  else if(dieing_proc.num_burst <= 0) //The proc is dead
  {
    printf("time %lums: P%i completed I/O %s\n", time, dieing_proc.proc_num, 
  	    PRINT_Q);
    End_of_Proc(dieing_proc);
  }
  else
  {
  	proc_q.add_proc( dieing_proc);
  	printf("time %lums: P%i completed I/O %s\n", time, dieing_proc.proc_num, 
  	    PRINT_Q);
  	//DO I note this?
  }
}

void Cpu::IO_dealings()
{
  //increments all things in IO;
  for(std::list<Proc>::iterator it= io_now.begin(); it != io_now.end() ; ++it )
  {
  	if(it->io_time == 0 )
  	{
  	  end_of_IO(*it);
  	  it = io_now.erase(it);
  	}
  }
}



void Cpu::execute_ticking()
{
  time++;
  increment_cores();
  increment_IO();
  proc_q.increment();
  return;
}

void Cpu::execute_checking()
{
  cores_check_all();
  IO_dealings();
  //This will be more complicated with I/O after end of all procs
  return;
}

Result Cpu::execute_run()
{
  Result Ret; 
  time = -1; 
  //printf( "HERE %s\n", PRINT_Q);

  while( not_done() && time < 200)
  {
    //printf( "FUCK\n");
    //PRINT_Q;
    execute_ticking();
    // ummm
    //Everything is now up to time;
    // All we need to do is check that we're working


    execute_checking();
  

  }
  Ret = Run_result;
  return Ret;
} 