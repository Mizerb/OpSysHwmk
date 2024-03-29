// INCLUDES *****************************************************************************
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <list>



// DEFINES ******************************************************************************

#define file_name "processes.txt"



#define DEBUG

#define n  //how the fk is this a tunable constant? 
           //Detrimed when file is read, held as sizeof the list
#define m 1


#include "cpu.h"
// **************************************************************************************



void test()
{
  const char D[] = "r";
  FILE* meh = fopen("text.txt", D);
  char buff[255];
  int a, b, c;
  while( fgets(buff, 255 , meh) != NULL )
  {
    if( strstr(buff, "#") != NULL) continue;
    sscanf(buff, "%i|%i|%i", &a,&b,&c );
    printf("%i %i %i \n" , a,b, c);
  }
  fclose(meh);
}



int main( int argc , char* argv[])
{
  Cpu my_cpu(m);
  const char r[] = "r";
  const char w[] = "w";
  
  if( argc != 3)
  { 
    fprintf(stderr,"Hey, wrong inputs buddy\n");
    return 1;
  }
  

  
  FILE * fp = fopen( argv[1], r);
  if( fp == NULL)
  {
    fprintf( stderr , "Cannont open in file\n");
    return 0;
  }
  my_cpu.queue_populate( fp);

  fclose(fp);

  my_cpu.change_type(FCFS_);
  my_cpu.reset();
  
  Result FCFS_result = my_cpu.RUN();
  printf("\n");

  //FCFS_result.write_out( stderr, "JEB");

#ifdef DEBUG
  //printf("Run 1 executed\n");
  
  FCFS_result.print_me();
#endif
  my_cpu.reset();
  my_cpu.change_type(SRT_);

  Result SRT_result = my_cpu.RUN();
  printf("\n");
#ifdef DEBUG
  //printf("Run 2 executed\n");

  SRT_result.print_me();
#endif

  my_cpu.reset();
  my_cpu.change_type(RR_);

  Result RR_result = my_cpu.RUN();
#ifdef DEBUG
  //printf("Run 2 executed\n");

  RR_result.print_me();
#endif

  FILE * of = fopen( argv[2], w);
  if( of == NULL)
  {
    fprintf(stderr, "Can not create out file\n");
  }

  FCFS_result.write_out( of, "FCFS");
  SRT_result.write_out( of,  "SRT");
  RR_result.write_out( of, "RR");

  fclose(of);
  
  

  return 0;
}
