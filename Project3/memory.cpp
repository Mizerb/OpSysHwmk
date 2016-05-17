#include "memory.h"

void memory::print()
{
  for(int i = 0; i < x_size; i++) printf("=");
  printf("\n");
  for(int i = 0; i < y_size; i++ )
  {
    for(int j = 0 ; j < x_size; j++)
    {
      printf("%c", Mem[ (i*x_size)+j]);
    }
    printf("\n");
  }
  for(int i = 0; i < x_size; i++) printf("=");
  printf("\n");
}

bool memory::remove_program( char to_rmv)
{
  bool ret = false;
  for( unsigned i = 0 ; i < Mem.size() ; i++)
  {
    if(Mem[i] == to_rmv)
    {
      //printf( "found 1 at %u \n" , i);
      Mem[i] = '.';
      ret = true;
    }
    //printf( "%u\n", i);
  }
  //printf("shit\n");
  for( unsigned i = 0 ; i < active_proc.size() ; i++)
  {
    if( active_proc[i].name == to_rmv)
      check_proc_empty( i);
  }
  return ret;
}

int memory::find_first_fit( int size_to_fit)
{
  int  space = 0;
  for( unsigned i = 0 ; i < Mem.size() ; i++)
  {
    if( Mem[i] == '.' )
    {
      while( i < Mem.size() && Mem[i] == '.')
      {
        space++; i++;
      }
      if( space >= size_to_fit) return i - space;
    }
    space = 0;
  }
  return -1;
}

int memory::find_next_fit( int size_to_fit)
{
  if( (unsigned)(next + size_to_fit) >  Mem.size() ) next = 0;

  int space = 0; //printf( "next : %d\n" ,next);
  for( unsigned i = next; i < next + Mem.size() ; i++)
  {
    //printf("%u \n", i);
    if(Mem[ i%(Mem.size()) ] == '.')
    {
      while( i !=  Mem.size() && Mem[i%(Mem.size())] == '.' )
      {
        //printf("%u \n", i);
        space++; i++;
        if( space >= size_to_fit )
        {
          next = i%(Mem.size());
          return (i%(Mem.size()))-space;
        } 
      }
    }
    space = 0;
  }
  return -1;
}

int memory::find_best_fit( int size_to_fit)
{
  int  space = 0 , ret =  (int) Mem.size() * 2 , pos= -1;
  for( unsigned i = 0 ; i < Mem.size() ; i++)
  {
    if( Mem[i] == '.' )
    {
      while( i < Mem.size() && Mem[i] == '.')
      {
        space++; i++;
      }
      if( space >= size_to_fit && space < ret )
      {
        pos = i - space;
        ret = space;
      } 
    }
    space = 0;
  }
  return pos;
}

int memory::open_memory()
{
  int Ret =0;
  for( unsigned i = 0 ; i < Mem.size() ; i++)
    if( Mem[i] == '.' ) Ret++;
  
  return Ret;
}

int memory::largest_space()
{
  int Ret = -1 ; int count =0;
  for (unsigned i = 0; i < Mem.size(); ++i)
  {
    if( Mem[i] != '.' )
    {
      if( count > Ret) Ret = count;
      count = 0;
    }
    count++;
  }
  if( count > Ret) Ret = count;
  //printf( "largest_space %d open memory %d\n", Ret , open_memory());
  return Ret;
}


// shit shit shit, best fit solution?!?!?
int memory::check_space( int start, int size)
{
  int free_space_count = 0;
  for( unsigned i = start; i < Mem.size() ; i++)
  {
    if( Mem[i] != '.') return free_space_count;
  }
  return free_space_count;
}

int memory::check_space_overflow( int start )
{
  int free_space_count = 0;
  for( unsigned i = start ; i < (start + Mem.size()) ; i++ )
    return free_space_count;
  return -1;
}

void memory::insert_proc( proc &to_add , int local , int current_time )
{
  //printf("START: %d , END %d \n", local , to_add.size);
  for( unsigned i = local ; i < (unsigned)(local+to_add.size) ; i++)
  {
    Mem[i] = to_add.name;
     //printf("%c", Mem[i]);
  }
  to_add.remaining_time = to_add.timing.front().second - to_add.timing.front().first;
  //printf("PRoc %c: remaining time %d\n", to_add.name, to_add.remaining_time  );
  to_add.timing.pop_front();
}

void memory::insert_proc_brute( proc &to_add)
{
  int remaining = to_add.size;
  for( unsigned i = 0 ; i < Mem.size(); i++)
  {
    if( remaining == 0) break;
    if( Mem[i] == '.')
    {
      Mem[i] = to_add.name;
      remaining--;
    } 
  }
  to_add.remaining_time = to_add.timing.front().second - to_add.timing.front().first;
  to_add.timing.pop_front();
}

int memory::add_proc_check(int timing)
{
  for( unsigned i = 0 ; i < active_proc.size() ; i++)
  {
    if( active_proc[i].timing.size() == 0) continue;
    if( active_proc[i].timing.front().first == timing)
    {
      return i;
    }
  }
  return -1;
}

void memory::check_proc_empty( int next_proc)
{
  if( active_proc[next_proc].timing.empty() == true)
   active_proc.erase(active_proc.begin()+ next_proc);
}

char memory::check_for_removal()
{
  for( unsigned i = 0 ; i < active_proc.size() ; i++)
  {
    if( active_proc[i].remaining_time <= 0  && 
      active_proc[i].remaining_time != -888)
    {
      active_proc[i].remaining_time = -888;
      return active_proc[i].name;
    }
  }
  return 0;
}

void memory::increment_procs()
{
  for (unsigned i = 0; i < active_proc.size() ; ++i)
  {
    if( active_proc[i].remaining_time > 0 )
      active_proc[i].remaining_time--;
  }
}

void memory::Defrag( int &local_time, int size)
{
  char n = 0x00; unsigned j , count = 0; std::vector<char> names;
  for( unsigned i = 0 ; i < Mem.size() ; i++)
  {
    if( Mem[i] == '.') 
    {
      for( j = i+1; j < Mem.size() ; j++)
      {
        if( Mem[j] != '.') break;
      }
      n = Mem[j];
      names.push_back(n);
      for( ; j < Mem.size() ; j++ , i++)
      {
        if( Mem[j] != n) break;
        Mem[i] = Mem[j];
        Mem[j] = '.';
        local_time++;
        count++;
      }
      i--;
      if( largest_space() >= size) break;
    }
  }
  printf("time %dms: Defragmentation complete (moved %u frames: ",
    local_time , count);
  for( unsigned i = 0 ; i < names.size() ; i++)
  {
    if( i == 0) printf("%c", names[i]);
    else printf(", %c", names[i]);
  }
  printf(")\n");
}

void memory::Simmulate_First_Fit()
{
  printf("time 0ms: Simulator started (Contiguous -- First-Fit)\n");
  int end_time = execute_sim( 0 );
  printf("time %dms: Simulator ended (Contiguous -- First-Fit)\n\n", end_time);
  return;
}

void memory::Simmulate_Next_Fit()
{
  printf("time 0ms: Simulator started (Contiguous -- Next-Fit)\n");
  next = 0;
  int end_time = execute_sim( 1 );
  printf("time %dms: Simulator ended (Contiguous -- Next-Fit)\n\n", end_time);
  return;
}

void memory::Simmulate_Best_Fit()
{
  printf("time 0ms: Simulator started (Contiguous -- Best-Fit)\n");
  int end_time =  execute_sim( 2 );
  printf("time %dms: Simulator ended (Contiguous -- Best-Fit)\n\n", end_time);
  return;
}

void memory::Simmulate_Non_Cont()
{
  printf("time 0ms: Simulator started (Non-contiguous)\n");
  int end_time =  execute_sim ( 3);
  printf("time %dms: Simulator ended (Non-contiguous)\n\n", end_time);
  return;
}

int memory::execute_sim( int mode)
{
  int local_time = 0 , procing_time = 0;
  active_proc = procs;
  //printf("active_proc size %d\n", (int)procs.size());
  while( not active_proc.empty() )
  {
    //REMOVE THE PROCS
    while( 1 )
    {
      char a = check_for_removal();
      if( a == 0 ) break;
      remove_program( a);
      printf("time %dms: Process %c removed from physical memory\n", 
        local_time , a);

      this->print();
    }

    //Do the adding of the procs
    // Well the checking of the rpocs as well
    //Esenntallly do all the things....
    while( 1)
    {
      int next_proc = add_proc_check( procing_time );
      if( next_proc == -1 ) break;
      printf("time %dms: Process %c arrived (requires %d frames of physical memory)\n",
        local_time , active_proc[next_proc].name , active_proc[next_proc].size );  
      int pos;
      //this->print();
      if( open_memory() < active_proc[next_proc].size )
      {
        //reject program. It cannot run.
        printf("time %dms: Cannot place process %c -- skipping process %c\n",
          local_time , active_proc[next_proc].name , active_proc[next_proc].name);
        active_proc[next_proc].timing.pop_front();
        check_proc_empty( next_proc);
        this->print();
        break;
      }

      if( mode != 3 ) // DEFRAG THE THINGS
      {
        if(largest_space() <  active_proc[next_proc].size)
        {
          printf("time %dms: Cannot place process %c -- starting defragmentation\n",
            local_time , active_proc[next_proc].name);
          Defrag(local_time , active_proc[next_proc].size);
          this->print();
        } 
      }
      // INsertion is the main thing that changes, so that's the only chnage
      // between modes, other than positon selection
      switch ( mode)
      {
        case 0: 
          pos = find_first_fit( active_proc[next_proc].size);
          insert_proc( active_proc[next_proc], pos , local_time);
          break;
        case 1:
          //printf("next : %d\n" , next);
          pos = find_next_fit( active_proc[next_proc].size );
          next = pos + active_proc[next_proc].size;
          //printf("pos : %d \n", pos);
          insert_proc( active_proc[next_proc], pos , local_time);
          break;
        case 2:
          pos = find_best_fit( active_proc[next_proc].size);
          //printf("pos %d\n", pos);
          insert_proc( active_proc[next_proc], pos , local_time);
          break;
        case 3:
          insert_proc_brute( active_proc[next_proc]);
          break;
      }

      printf("time %dms: Placed process %c in memory:\n", local_time, active_proc[next_proc].name);
      this->print();
      
      //active_proc[i]  
    }
    

    increment_procs();
    local_time++;
    procing_time++;
    //if( local_time > 2834 ) printf("size = %d\n", (int)active_proc.size() );
  }
  return --local_time;
}