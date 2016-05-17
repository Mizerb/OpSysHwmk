#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "memory.h"
#include "virt_mem.h"


#define X_SIZE 32
#define Y_SIZE 8
#define F 3
//The lazy way

#define T_MEM_MOVE 1

void readfile( char * filename , memory * the_memory)
{
  char BUFF[1024];
  int z; char n; int m;


  if (FILE *fp = fopen(filename, "r"))
  {
    fscanf(fp , "%d" , &m);
    
    fscanf(fp , "%s", BUFF);
    for(int i = 0; i< m ; i++)
    {
      std::list<std::pair<int,int> > timing;  
      sscanf(BUFF ,"%c" , &n);
      
      fscanf(fp , "%s", BUFF);
      sscanf(BUFF ,"%d", &z );

      //printf("%s\n", BUFF);
      while( fscanf(fp , "%s", BUFF) != EOF && strlen(BUFF) > 1)
      {
        int str, fin;
        //printf("%s\n", BUFF);
        sscanf(BUFF, "%d-%d", &str, &fin);
        timing.push_back(std::make_pair(str,fin));
      }
      //printf("ADDING %c : %d\n" , n , z);      
      the_memory->add_proc(n , z, timing );
    }

    fclose(fp);
  }
  else
  {
    fprintf(stderr, "Unable to open file 1\n");
    exit(1);
  }
  //printf("%d memm\n", (int) the_memory->procs.size());
}


int main(int argc, char *argv[])
{
  if( argc < 3)
  {
    fprintf(stderr, "Not enough args\n");
    return -1;
  } 

  memory * main_mem = new memory( X_SIZE, Y_SIZE);

  readfile( argv[1] , main_mem);


  
  main_mem->Simmulate_First_Fit();
  main_mem->Simmulate_Next_Fit();
  main_mem->Simmulate_Best_Fit();

  main_mem->Simmulate_Non_Cont();

  
  
  virt_mem * Virtual = new virt_mem( F);
  //  Not sure why I did the read in completly different way,
  //but I did. So, you know varaity. Not sure which is BEST
  //however...
  Virtual->Read_Ref(argv[2]);

  Virtual->Run_OPT_sim();
  Virtual->Run_LRU_sim();
  Virtual->Run_LFU_sim();

  delete main_mem;
  delete Virtual;
  return 0;
}