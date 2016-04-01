/* cpu .cpp */ 

#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "cpu.h"

#define BUFF_LEN 255

#define PRINT_Q (proc_q.print_Q()).c_str()

Cpu::Cpu()
{
  time =0;
  type = 0;
  Proc_Queue temp(SRT_);
  arrival_q = temp;
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

  printf("time %lums: Simulator ended for %s\n",
    time, (proc_q.print_type()).c_str());

  return Ret;
}

void Cpu::queue_populate( FILE * fp)
{
  //const char r[] = "r";
  char buff[BUFF_LEN];
  Proc_list new_q;
  while( fgets(buff, BUFF_LEN , fp ))
  {
  	int a_time;
    if( strstr(buff, "#") != NULL) continue;
  	Proc new_proc;
  	sscanf(buff, "%i|%i|%i|%i|%i", 
  	    &(new_proc.proc_num) , 
        &(a_time),
  	    &(new_proc.inital_burst_time), 
  	    &(new_proc.inital_num_burst) , 
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
    new_proc.num_burst = new_proc.inital_num_burst;
  	new_q.Add_proc(new_proc , a_time);
  }
  inital_set = new_q;
  return;
}

bool Cpu::not_done()
{
  if(Run_result.task_count == inital_set.get_size() )
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
  inital_set = working_set;
  Result meh;
  Run_result = meh;
  Core temper;
  std::list<Core> temp(cores.size(), temper);
  cores = temp;
  time = 0;  //just to be sure
}

void Cpu::change_type( int i)
{
  Proc_Queue new_queue(i);
  proc_q = new_queue;
  type = i;
  return;
}

void Cpu::queue_working()
{
  while(working_set.next_proc(time))
  {
    Proc a_proc = working_set.get_next_proc();
    proc_q.add_proc(a_proc);
    printf("time %lums: P%i arrived %s\n", time, a_proc.proc_num,
      PRINT_Q);
  }
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
  if(type == FCFS_ ) core_walk();
  if(type == SRT_ ) SRT_core_walk();
  if(type == RR_ ) RR_core_walk();
}

void Cpu::SRT_core_walk()
{
  core_walk();
  bool swapping_check = true;
  for(std::list<Core>::iterator it= cores.begin(); it != cores.end() ; ++it )
  {
    // Stupid pre-emptying thing, why do arrival times have to be delayed?
    swapping_check &= it->is_context_swapping;
    if( arrival_q.is_empty() || it->is_context_swapping) continue; // Done

    if(it->burst_now.burst_time < arrival_q.top().burst_time ) // Prempt
    {
      printf("time %lums: P%i arrived, preempting P%i %s\n", time, arrival_q.top().proc_num,
        it->burst_now.proc_num , PRINT_Q ); // Truly evil I know.

      proc_q.add_proc(it->burst_now);

      it->start_context_swap( arrival_q.get_next() );

      Run_result.context_swaps++;
    }
  }

  if(swapping_check)
  {
    while( !(arrival_q.is_empty()))
    {
      Proc next = arrival_q.get_next();
      proc_q.add_proc( next);
      printf("time %lums: P%i arrived %s\n", time, 
        next.proc_num , PRINT_Q ); // Truly evil I know.
    }
  }
}

void Cpu::RR_core_walk()
{
  for(std::list<Core>::iterator it= cores.begin(); it != cores.end() ; ++it )
  {
    if(it->time_expired())
    {
      if(proc_q.is_empty()) continue;
      //Premet this bitch
      
      printf("time %lums: Time slice expired, preempting P%i %s\n", time, 
        it->burst_now.proc_num , PRINT_Q ); // Truly evil I know.

      proc_q.add_proc(it->burst_now);

      it->start_context_swap( proc_q.get_next() );

      Run_result.context_swaps++;
    }
    else if(it->rdy_for_proc()) //Doesn't have a Proc
    {
      if(proc_q.is_empty()) continue;
      
      it->start_context_swap( proc_q.get_next() );

      Run_result.context_swaps++;
    }
    else if(it->rdy_to_start())
    {
      
      it->receive_proc( it->context_hold );
      
      printf("time %lums: P%i started using the CPU %s\n", time, 
        it->burst_now.proc_num , PRINT_Q ); // Truly evil I know.
    }
    else if(it->burst_now.burst_time == 0 && it->has_proc) //Get process out of CPU
    {
      it->wait_for_proc();
      burst_end(it->burst_now);
      if(proc_q.is_empty()) continue;
      it->start_context_swap(proc_q.get_next() );
      Run_result.context_swaps++;
    }
  }
}


void Cpu::core_walk()
{
  for(std::list<Core>::iterator it= cores.begin(); it != cores.end() ; ++it )
  {
    if(it->rdy_for_proc()) //Doesn't have a Proc
    {
      if(proc_q.is_empty()) continue;
      
      it->start_context_swap( proc_q.get_next() );

      Run_result.context_swaps++;
    }
    else if(it->rdy_to_start())
    {
      
      it->receive_proc( it->context_hold );
      
      printf("time %lums: P%i started using the CPU %s\n", time, 
        it->burst_now.proc_num , PRINT_Q ); // Truly evil I know.
    }
    else if(it->burst_now.burst_time == 0 && it->has_proc) //Get process out of CPU
    {
      it->wait_for_proc();
      burst_end(it->burst_now);
      if(proc_q.is_empty()) continue;
      it->start_context_swap(proc_q.get_next() );
      Run_result.context_swaps++;
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
  	sprintf(buff,"time %lums: P%i completed I/O %s\n", time, dieing_proc.proc_num, 
  	    PRINT_Q);
  }
  else if(dieing_proc.num_burst <= 0) //The proc is dead
  {
    sprintf(buff,"time %lums: P%i completed I/O %s\n", time, dieing_proc.proc_num, 
  	    PRINT_Q);
    End_of_Proc(dieing_proc);
  }
  else
  {
  	proc_q.add_proc( dieing_proc);
  	sprintf(buff,"time %lums: P%i completed I/O %s\n", time, dieing_proc.proc_num, 
  	    PRINT_Q);
  	//DO I note this?
  }
  io_stalling = buff;
}

void Cpu::IO_dealings()
{
  //increments all things in IO;
  io_stalling = "";
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
  arrival_q.increment();
  return;
}

void Cpu::execute_checking()
{
  queue_working();
  cores_check_all();
  IO_dealings();
  cores_check_all();
  printf("%s", io_stalling.c_str());
  //This will be more complicated with I/O after end of all procs
  return;
}

Result Cpu::execute_run()
{
  Result Ret; 
  time = -1; 
  //printf( "HERE %s\n", PRINT_Q);

  while( not_done() )
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