#include <list>
#include <string>
#include "Proc.h"
#include "Proc_list.h"


#ifndef PROC_QUEUE_H
#define PROC_QUEUE_H


class Proc_Queue
{
private:
  Proc_list init_list;
  std::list<Proc> queue;
  int type;
public:
  Proc_Queue(int type_){ type = type_;}
  Proc_Queue(){ type = 0;}
  void add_proc( Proc to_add);
  Proc get_next();
  
  void increment();
  
  void change_type( int type_);
  
  bool is_empty(){ return queue.empty();}

  int get_type(){ return type;}
  int get_size(){ return queue.size();}

  std::string print_Q();
  std::string print_type();
};




#endif