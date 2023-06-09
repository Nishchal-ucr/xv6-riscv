#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64 global_sys_calls_counter = 0;

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 sys_sysinfo(void)
{
  int n;
  argint(0, &n);
  return get_sysinfo(n,global_sys_calls_counter);
}


uint64
sys_procinfo(void)
{
  uint64 pinfo_pointer;
  argaddr(0, &pinfo_pointer);
  return get_procinfo(pinfo_pointer);
}
uint64 sys_hello(void) // hello syscall definition
{
  int n;
  argint(0, &n);
  print_hello(n);
  return 0;
}
  
// prints, for each process:
// 1) PID,
// 2) name in a parenthesis,
// 3) the ticket value, and,
// 4) the number of times it has been scheduled to run
int sys_sched_statistics(void)
{
return sched_statistics();
}
// sets the caller process’s ticket value to the given parameter.
int sys_sched_tickets(int n)
{
argint(0, &n);
return sched_tickets(n);
}
