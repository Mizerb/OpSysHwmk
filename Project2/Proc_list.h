/* PROC_LIST_H */

class Proc_list
{
private:
    std::list<std::tuple<int,Proc>> Proc_in_list;

public:
    bool new_proc(int time);
    Proc get_new_proc();
    void Add_proc(Proc a_proc , int time); 
}