// INCLUDES *****************************************************************************
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <list>
// DEFINES ******************************************************************************

#define file_name "processes.txt"

#define FCFS 0
#define SJF  1
// **************************************************************************************



class Proc
{
public:
  int proc_num;
  int burst_time;
  int num_burst;
  int io_time;
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

Proc_Queue::Proc_Queue( int  type_)
{  
  type = type_;
}

void Proc_Queue::add_proc( Proc to_add)
{
  queue.push_back( to_add);
  if( type == SJF)
  {
    queue.sort( Proc_compare_burst);
  }
  return;
}

Proc Proc_Queue::get_next()
{
  Proc Ret = queue.front();
  queue.pop_front();
  return Ret;
}



class Cpu
{ 
private:
  Proc /* * */ burst_now;
  Proc /* * */ io_now;  //I don't know if these will be pointers. 
  unsigned long time;
  Proc_Queue * proc_q; 
  
  int type;

  void execute_run( std::ostream& out_put);

  void execute_tick();
   
  bool not_done(){return true;}  
public:
  Cpu(); // Holy shit, gotta make an object
  void queue_populate( std::istream& in_stream);
  
};

void Cpu::queue_populate( std::istream& in_stream)
{
  return;
}

void Cpu::execute_tick()
{
  time++;
  return;

}

void Cpu::execute_run(  std::ostream& out_put)
{
  while( not_done())
  {
      
  }
  return;
} 

int main( int argc , char* argv[])
{
  if( argc != 3)
  { 
    fprintf(stderr,"Hey, wrong inputs buddy\n");
    return 1;
  }
  return 0;
}



