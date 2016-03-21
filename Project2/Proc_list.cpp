/* PROC_LIST_CPP 
* BEN MIZERA
*/
#include "Proc_list.h"

bool I_feel_bad( const struct tied &i, const struct tied &j)
{
  return ( i.the_time < j.the_time);
}

void Proc_list::finalize() //make it neat
{
	list.sort( I_feel_bad );
	return;
} 

bool Proc_list::next_proc(int time)
{
    if( list.front().the_time == time ) return true;
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
    list.push_back( new_tie);
}