/* PROC_LIST_CPP 
* BEN MIZERA
*/
#include "Proc_list.h"
#include <stdio.h>

bool I_feel_bad( const struct tied &i, const struct tied &j)
{
  return ( i.the_time < j.the_time);
}

void Proc_list::finalize() //make it neat
{
	list.sort( I_feel_bad );
	return;
} 

bool Proc_list::next_proc(unsigned long time)
{
    //printf("%lu COMPARE %i" , time, list.front().the_time);
    if( (unsigned long)list.front().the_time <= time ) return true;
    else return false;
} 

Proc Proc_list::get_next_proc()
{
    Proc ret = list.front().the_proc;
    list.pop_front();
    return ret;
}

void Proc_list::Add_proc(Proc a_proc , int a_time)
{
    tied new_tie; new_tie.the_proc = a_proc; new_tie.the_time = a_time;
    //printf("Here, time is : %i" , a_time);
    list.push_back( new_tie);
}

void Proc_list::PRINT()
{
  for(std::list<tied>::iterator it= list.begin(); it != list.end() ; ++it )
  {
  	printf( "P%i: ARRIVAL: %i\n" , it->the_proc.proc_num, it->the_time);
  	 // Yeah, something was broken
  	// with std::to_string
  }
}