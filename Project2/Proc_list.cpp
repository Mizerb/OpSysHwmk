/* PROC_LIST_CPP 
* BEN MIZERA
*/
#include <Proc_list.h>


bool Proc_list::next_proc(int time)
{
    if( std::get<0>(list.front()) == time ) return true;
    else return false;
} 

Proc Proc_list::get_next_proc()
{
    Proc ret = std::get<1>(list.front());
    list.pop_front();
    return ret;
}

void Proc_list::Add_proc(Proc a_proc , int time)
{
    list.push_back( std::tuple<int,Proc> (time , a_proc ));
}