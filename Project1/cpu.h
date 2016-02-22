#include "proc_queue.h"
#include <iostream>

#ifndef CPU_H
#define CPU_H

class Core
{
public:
  Core(){ is_context_swapping = true; context_countdown = t_cs+1;}

  Proc burst_now;
  bool is_context_swapping;
  int context_countdown;

  bool rdy_for_proc();
  
  void receive_proc( Proc new_proc);

  void increment();
  void start_context_swap();
};

void Core::increment()
{
  if(is_context_swapping) context_countdown--;
  else burst_now.in_cpu_incre();
}

void receive_proc( Proc new_proc)
{
  burst_now = new_proc;
  is_context_swapping = false;
}

bool Core::rdy_for_proc()
{
  return (is_context_swapping && (context_countdown == 0));
}

void Core::start_context_swap()
{
  is_context_swapping = true;
  context_countdown = t_cs;
  return;
}


class Result
{
public:
  Result(){ task_count = CPU_burst_time = total_wait_time = total_turn_time = context_swaps =0;}
  int CPU_burst_time;
  int total_wait_time;
  int total_turn_time;
  int context_swaps;

  int task_count;

  void print_me();

  void write_out( FILE* of, std::string name);

};

class Cpu
{ 
private:
  Proc /* * */ burst_now;
  std::list<Core> cores;
  std::list<Proc> /* * */ io_now;  //I don't know if these will be pointers. 
  
  unsigned long time;
  
  Result Run_result;

  Proc_Queue proc_q; 
  Proc_Queue inital_q;
  
  Result my_result;

  Result execute_run();

  void execute_tick();

  void increment_cores();
  void increment_IO();

  bool not_done();
  void new_io(Proc new_proc);
  void IO_dealings();
public:
  Cpu(); // Holy shit, gotta make an object
  Cpu(int _core_count)
  Result RUN();
  void queue_populate( FILE* fp);
  void reset();
  void change_type( int i);
  
};

#endif 