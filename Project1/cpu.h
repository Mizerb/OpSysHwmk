#include "proc_queue.h"
#include <iostream>

#ifndef t_cs
#define t_cs 9
#endif


#ifndef CPU_H
#define CPU_H

class Core
{
public:
  Core(){ is_context_swapping = false; context_countdown = t_cs+1; 
    has_proc =false;}

  Proc burst_now;
  bool is_context_swapping;
  int context_countdown;

  bool has_proc;

  bool rdy_for_proc();
  bool rdy_to_start();
  
  void receive_proc( Proc new_proc);
  void wait_for_proc();

  void increment();
  void start_context_swap();
};



class Result
{
public:
  Result(){ task_count = CPU_burst_time = total_wait_time = total_turn_time = context_swaps =0;}
  int CPU_burst_time;
  int total_wait_time;
  int total_turn_time;
  int context_swaps;

  int task_count;
  
  void add_proc( Proc dead_proc);

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

  Result execute_run();

  void execute_ticking();
  void execute_checking();

  void increment_cores();
  void cores_check_all();
  void End_of_Proc(Proc dead_proc);

  void new_io(Proc new_proc);
  void increment_IO();
  void IO_dealings();
  void end_of_IO(Proc dieing_proc);

  bool not_done();
  void burst_end( Proc dead_proc);

public:
  Cpu(); // Holy shit, gotta make an object
  Cpu(int _core_count);
  Result RUN();
  void queue_populate( FILE* fp);
  void reset();
  void change_type( int i);
  
};

#endif 

