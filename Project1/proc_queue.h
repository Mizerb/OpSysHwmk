#include <list>


#ifndef PROC_QUEUE_H
#define PROC_QUEUE_H

class Proc
{
public:
  Proc()
  { proc_num = burst_time = inital_burst_time = num_burst =inital_io_time =0;}
  int proc_num;
  int burst_time;
  int inital_burst_time;
  int num_burst;
  int inital_io_time;
};

bool Proc_compare_burst( const Proc & i , const Proc & j)
{
  if( i.burst_time == j.burst_time ) return (i.proc_num < j.proc_num);
  else return ( i.burst_time < j.burst_time );
}

bool Proc_compare_proc_numb( const Proc &i, const Proc &j)
{
  return ( i.proc_num < j.proc_num);
}


class Proc_Queue
{
private:
  std::list<Proc> queue;
  int type;
public:
  Proc_Queue(int type_);
  ~Proc_Queue();
  void add_proc( Proc to_add);
  Proc get_next();
};






#endif