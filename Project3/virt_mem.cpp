#include "virt_mem.h"


void virt_mem::Read_Ref( char * file_name)
{
  //std::vector<char> v;
  int meh;
  if (FILE *fp = fopen(file_name, "r"))
  {
    //char buf[1024];
    while( fscanf( fp , "%d", &meh) != EOF )
    {
      reference_pages.push_back( meh);
    }
    fclose(fp);
  }
}

void virt_mem::print_mem()
{
  printf("[mem:");
  for( unsigned i = 0 ; i < Mem.size() ; i++)
  {
    if( Mem[i] == 0) printf(" .");
    else printf(" %d", Mem[i]);
  }
  printf("]");
}

bool virt_mem::contains( int page)
{
  for( unsigned i = 0 ; i < Mem.size() ; i++)
  {
    if( Mem[i] == page) return true;
  }
  return false;
}
 
void virt_mem::update_vict( int mode)
{
  //thingy
  if( mode != 2 ) return;

  for( unsigned i = 0 ; i < Mem.size() ; i++)
  {
    if( Mem[i] == reference_pages[position])
    {
      Mem_vict[i]++;
      return;
    }
  }
  printf("NOT SUPPOSED TO BE HERE\n");
}

int virt_mem::OPT_select()
{
  unsigned high = 0  ; int pos = -1;
  //printf("position: %u\n", position);
  for( unsigned i = 0 ; i< Mem.size() ; i++)
  {
    unsigned j = position; bool seen = false;
    for( j = position; j < reference_pages.size(); j++)
    {
      if( reference_pages[j] == Mem[i] )
      {
        if( j > high)
        {
          high = j;
          pos = i;
        }
        seen = true;
        break;
      }
    }
    //printf( "high: %u j: %u \n", high , j);
    if( (not seen) )
    {
      if( pos == -1)
        pos = i;
      else if( Mem[i] < Mem[pos] || j > high)
        pos = i;
        high = j;
    }
  }
  return pos;
}

int virt_mem::LRU_select()
{
  unsigned low = (int)reference_pages.size(); int pos =0;
  for(unsigned i = 0 ; i< Mem.size() ; i++)
  {
    unsigned j = position;
    while( j--)
    {
      if( reference_pages[j] == Mem[i] )
      {
        if( j < low )
        {
          pos = i;
          low = j;
        } 
        break;
      }
    }
  }
  //printf("pos: %d\n", pos);
  return pos;
}

int virt_mem::LFU_select()
{
  int low = (int)reference_pages.size() , pos;
  for( unsigned i = 0 ; i < Mem.size() ; i++)
  {
    if( Mem_vict[i] < low)
    {
      pos = i; low = Mem_vict[i];
    }
    else if( Mem_vict[i] == low &&  Mem[i] < Mem[pos])
    {
      pos = i;
    }
  }
  Mem_vict[pos] = 1;
  return pos;
}

int virt_mem::select_vict( int mode )
{
  for( unsigned i = 0 ; i < Mem.size() ; i++)
  {
    if( Mem[i] == reference_pages[position] || Mem[i] == 0)
     return i;
  }


  int ret;
  switch (mode)
  {
    case 0:
      ret = OPT_select();
      break;
    case 1:
      ret = LRU_select();
      break;
    case 2:
      ret = LFU_select();
      break;
  }
  return ret;
}

void virt_mem::Run_OPT_sim()
{
  printf("Simulating OPT with fixed frame size of %d\n", frames );
  Run_sim(0);
  printf("End of OPT simulation (%d page faults)\n\n", page_fault_count);
}

void virt_mem::Run_LRU_sim()
{
  printf("Simulating LRU with fixed frame size of %d\n", frames );
  Run_sim(1);
  printf("End of LRU simulation (%d page faults)\n\n", page_fault_count);
}

void virt_mem::Run_LFU_sim()
{
  printf("Simulating LFU with fixed frame size of %d\n", frames );
  Run_sim(2);
  printf("End of LFU simulation (%d page faults)\n", page_fault_count);
}

void virt_mem::Run_sim( int mode)
{
  std::vector<int> a(frames , 0);
  Mem  = a;
  Mem_vict = std::vector<int>(frames , 1);
  page_fault_count = 0;
  for( position = 0 ; position < reference_pages.size() ; position++)
  {
    printf("referencing page %d ", reference_pages[position]);

    //Life is good
    if( this->contains(reference_pages[position]) )
    {
      update_vict(mode);

      print_mem();
      printf("\n"); // Weird formating
      continue; //Next page
    }

    int rmv_pg = select_vict( mode );
    int vict;
    if( Mem[rmv_pg] == 0)
      vict = -1;
    else vict = Mem[rmv_pg];
    Mem[rmv_pg] = reference_pages[position];
    print_mem();
    printf( " PAGE FAULT ");
    page_fault_count++;
    if( vict == -1)
    {
      printf("(no victim page)\n");
    }
    else
    {
      printf("(victim page %d)\n", vict);
    }
  }
}