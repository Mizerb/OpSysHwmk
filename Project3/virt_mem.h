#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class virt_mem
{
public:
  int frames;  
  int page_fault_count;
  unsigned position;


  virt_mem( int frames_ ): frames(frames_) {}
  
  std::vector<int> reference_pages;
  

  std::vector<int> Mem;
  std::vector<int> Mem_vict;

  void Read_Ref( char * file_name);
  void print_mem();

  bool contains( int page);
  
  int select_vict( int mode); 
  int OPT_select();
  int LRU_select();
  int LFU_select();
 
  void update_vict(int mode);

  void Run_OPT_sim();
  void Run_LRU_sim();
  void Run_LFU_sim();

  void Run_sim( int mode);

};