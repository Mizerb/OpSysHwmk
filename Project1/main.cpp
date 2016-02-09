// INCLUDES *****************************************************************************
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// DEFINES ******************************************************************************

#define file_name "processes.txt"

// **************************************************************************************



class Proc
{
  int proc_num;
  int burst_time;
  int num_burst;
  int io-time;
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

class Cpu
{
  Proc * burst_now;
  Proc * io_now;
  unsigned long time;
  std::vector<process> process_queue; 
  
  void queue_populate( std::istream& in_stream);
};

void Cpu::queue_populate( std::istream& in_stream)
{
  return;
}



void execute_FCFS( CPU cpu , std::ostream% out_put)
{
  return;
} 


void execute_SJF( CPU cpu , std::ostream& out_put)
{
  std::sort(cpu.process_queue.begin() , cpu.process_queue.end(), 


  return;
}


int main( int argc , char* argv[])
{
  

  return 0;
}



