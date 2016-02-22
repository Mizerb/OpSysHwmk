/* CORE.cpp */ 
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "cpu.h"

/*
void Core::increment()
{
  if(is_context_swapping)
  {
  	context_countdown--;
  	burst_now.is_context_swap();
  } 
  else burst_now.in_cpu_incre();
}

void Core::receive_proc( Proc new_proc)
{
  burst_now = new_proc;
  is_context_swapping = false;
  has_proc = true;
}

bool Core::rdy_for_proc()
{
  return (!has_proc && !is_context_swapping);
}

bool Core::rdy_to_start()
{
  return (is_context_swapping && (context_countdown==0));
}

void Core::start_context_swap()
{
  is_context_swapping = true;
  context_countdown = t_cs;
  has_proc = false;
  return;
}

void Core::wait_for_proc()
{
  is_context_swapping = false;
  has_proc = false;
}

*/
void Core::increment()
{
  if(is_context_swapping)
  {
  	context_countdown--;
  	burst_now.is_context_swap();
  } 
  else burst_now.in_cpu_incre();
}

void Core::receive_proc( Proc new_proc)
{
  burst_now = new_proc;
  is_context_swapping = false;
  has_proc = true;
}

bool Core::rdy_for_proc()
{
  return (!has_proc && !is_context_swapping);
}

bool Core::rdy_to_start()
{
  return (is_context_swapping && (context_countdown==0));
}

void Core::start_context_swap()
{
  is_context_swapping = true;
  context_countdown = t_cs;
  has_proc = false;
  return;
}

void Core::wait_for_proc()
{
  is_context_swapping = false;
  has_proc = false;
}
