#ifndef MEM_H
#define MEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <list>

class proc
{
public:
  proc(char n , int size_): name(n) , size(size_) {}
  
  char name;
  int size;
  int remaining_time;
  std::list<std::pair<int,int> > timing; //entry exit

};


class memory
{
public:
  memory(int x_size_ , int y_size_): x_size( x_size_) , y_size(y_size_) {
    std::vector<char> meh( x_size * y_size, '.');
    Mem = meh;
  }
  
  int x_size;
  int y_size;
  
  std::vector<char> Mem;
  std::vector<proc> procs;
  std::vector<proc> active_proc;
  int next;

  void add_proc( char n , int size, std::list<std::pair<int,int> > timing_)
  {
    proc my_proc(n , size);
    my_proc.timing = timing_;
    my_proc.remaining_time = -888;
    procs.push_back(my_proc); 
  }

  int add_proc_check( int timing);

  int find_first_fit( int size_to_fit);
  int find_next_fit( int size_to_fit);
  int find_best_fit( int size_to_fit);

  bool remove_program( char to_rmv);

  int check_space( int start, int size);
  int check_space_overflow(int start);
  void Defrag( int &local_time , int size);

  void insert_proc( proc &to_add , int local);
  void insert_proc_brute( proc &to_add);

  void increment_procs();
  int open_memory();
  int largest_space();

  void print();

  void insert_proc( proc &to_add , int local , int current_time);

  void Simmulate_First_Fit();
  void Simmulate_Next_Fit();
  void Simmulate_Best_Fit();

  void Simmulate_Non_Cont();

  char check_for_removal();
  void check_proc_empty( int next_proc);

  int execute_sim( int mode);

};
#endif