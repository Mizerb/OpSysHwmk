/* PROC_LIST_CPP 
* BEN MIZERA
*/
#include <Proc_list.h>

struct Feels_like_cheating
{
	bool operator()(tuple<int, Proc> const &lhs,tuple<int, Proc> const & rhs)
	{
		return std::get<0>(lhs) < std::get<1>(rhs);
	}
};

void Proc_list::finalize() //make it neat
{
	std::sort(list.begin() , list.end());
	return;
} 

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

void Proc_list::Add_proc(Proc a_proc , int a_time)
{
    list.push_back( std::tuple<int,Proc> (a_time , a_proc ));
}