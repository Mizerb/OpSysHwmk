// INCLUDES *****************************************************************************
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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
  std::list<Proc>* queue;
public:
  Proc_Queue();
  ~Proc_Queue(){ delete queue; }
  void add_proc( Proc to_add);
  Proc get_next();
}

Proc_Queue::Proc_Queue()
{
  this.queue = new 


class Cpu
{
  Cpu() 
private:
  Proc /* * */ burst_now;
  Proc /* * */ io_now;  //I don't know if these will be pointers. 
  unsigned long time;
  std::vector<Proc> process_queue; 
  
  int type;

  void execute_FCFS( std::ostream& out_put);
  void execute_SJF ( std::ostream& out_put);
  
public:
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

void Cpu::execute_FCFS(  std::ostream& out_put)
{
  while( Cpu.not_done())
  {
      
  }
  return;
} 

int main( int argc , char* argv[])
{
  

  return 0;
}



