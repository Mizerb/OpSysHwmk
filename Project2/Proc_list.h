/* PROC_LIST_H */

#include <tuple>

class Proc_list
{
private:
    std::list<std::tuple<int,Proc>> list;

public:
    bool next_proc(int time);
    Proc get_next_proc();
    void Add_proc(Proc a_proc , int time); 
}