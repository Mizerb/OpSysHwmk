#include <list>


#ifndef PROC_QUEUE_H
#define PROC_QUEUE_H

class Proc
{
public:
  Proc()
  { wait_time = turn_time = proc_num = burst_time = inital_burst_time = num_burst =inital_io_time =0;}
  int proc_num;
  int burst_time;
  int inital_burst_time;
  int num_burst;
  int io_time;
  int inital_io_time;
  int wait_time;
  int turn_time; 
};

bool Proc_compare_burst( const Proc & i , const Proc & j);

bool Proc_compare_proc_numb( const Proc &i, const Proc &j);

class Proc_Queue
{
private:
  std::list<Proc> queue;
  int type;
public:
  Proc_Queue(int type_){ type = type_;}
  Proc_Queue(){ type = 0;}
  void add_proc( Proc to_add);
  Proc get_next();
  
  void increment();
  
  void change_type( int type_){ type = type_;}
  
  bool is_empty(){ return queue.empty();}

  std::string print_Q();
};




#endif