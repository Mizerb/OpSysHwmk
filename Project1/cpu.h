


#ifndef CPU_H
#define CPU_H



class Cpu
{ 
private:
  Proc /* * */ burst_now;
  Proc /* * */ io_now;  //I don't know if these will be pointers. 
  unsigned long time;
  Proc_Queue proc_q; 
  Proc_Queue inital_q;

  void execute_run( std::ostream& out_put);

  void execute_tick();
   
  bool not_done(){return true;}  
public:
  Cpu(); // Holy shit, gotta make an object
  void queue_populate( std::istream& in_stream);
  void reset();
  void change_type();
  
};

#endif 