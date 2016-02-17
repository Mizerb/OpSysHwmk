#include "proc_queue.h"
#include <string>
#include <stdio.h>


bool Proc_compare_burst( const Proc & i , const Proc & j)
{
  if( i.inital_burst_time == j.inital_burst_time ) return (i.proc_num < j.proc_num);
  else return ( i.inital_burst_time < j.inital_burst_time );
}

bool Proc_compare_proc_numb( const Proc &i, const Proc &j)
{
  return ( i.proc_num < j.proc_num);
}




void Proc_Queue::add_proc(Proc to_add)
{
	queue.push_back( to_add);
	return;
}

Proc Proc_Queue::get_next()
{
	Proc Ret;

	//if( type == 0 ) queue.sort( Proc_compare_proc_numb);
	if( type == 1 ) queue.sort( Proc_compare_burst);

    Ret = queue.front();
    queue.pop_front();

	return Ret;
}

void Proc_Queue::increment()
{
  for(std::list<Proc>::iterator it= queue.begin(); it != queue.end() ; ++it )
  {
  	it->wait_time++; it-> turn_time++; 
  }
}


std::string Proc_Queue::print_Q() //Yeah, I'm sorry, I like the c outputs but
{                                // I want the ease of C++ strings... 
  std::string Ret = "[Q";
  char buff[255];
  for(std::list<Proc>::iterator it= queue.begin(); it != queue.end() ; ++it )
  {
  	Ret += " ";
  	sprintf(buff, "%d" , it->proc_num); // Yeah, something was broken
  	// with std::to_string
  	Ret += buff; 
  }
  
  Ret += "]";
  return Ret;
}