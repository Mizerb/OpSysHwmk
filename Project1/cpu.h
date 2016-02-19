#include "proc_queue.h"
#include <iostream>

#ifndef CPU_H
#define CPU_H

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
  std::list<Proc> /* * */ io_now;  //I don't know if these will be pointers. 
  unsigned long time;
  Proc_Queue proc_q; 
  Proc_Queue inital_q;
  
  Result my_result;

  int context_countdown;
  bool is_context_swapping;

  Result execute_run();

  void execute_tick();

  void thiser();

  bool not_done();
  void new_io(Proc new_proc);
  void IO_dealings();
public:
  Cpu(); // Holy shit, gotta make an object
  Result RUN();
  void queue_populate( FILE* fp);
  void reset();
  void change_type( int i);
  
};

#endif 