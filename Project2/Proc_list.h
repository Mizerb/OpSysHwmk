/* PROC_LIST_H */

#include "Proc.h"
#include <list>

struct tied
{
	int the_time;
	Proc the_proc;
};

class Proc_list
{
private:
    std::list<tied> list;

public:
    bool next_proc(int time);
    Proc get_next_proc();
    void Add_proc(Proc a_proc , int a_time); 
    void finalize();
    int get_size(){ return list.size();}
};