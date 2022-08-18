#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  int status;

 if( argint(0, &status) < 0 ){

   return -1;

 }
  exit(status);
  return 0;  // not reached
}

int
sys_wait(void)
{
   int *p;

 if(argptr(0, (void*)&p, sizeof(int))<0){

   return -1;

 }

  return wait(p);
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;
  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;

 
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
int
sys_helloWorld(void)
{
return helloWorld();
}
int
sys_numOpenFiles(int a)
{
   argint(0,&a);
   return numOpenFiles(a);

}
int
sys_memAlloc(int a)
{
 argint(0,&a);
	return memAlloc(a);
}

int
sys_getprocesstimedetails(int a)
{
  argint(0,&a);
	return getprocesstimedetails(a);
        //  struct proc *curproc=myproc();

   // cprintf("processCreationDateTime: %d:%d:%d:%d:%d:%d\n",curproc->pcr.second,curproc->pcr.minute,curproc->pcr.hour,curproc->pcr.day,curproc->pcr.month,curproc->pcr.year);
   // cprintf("processLastContextSwitchedOutDateTime: %d:%d:%d:%d:%d:%d\n",curproc->pso.second,curproc->pso.minute,curproc->pso.hour,curproc->pso.day,curproc->pso.month,;
}
int
sys_psinfo(void)
{
	return psinfo();
}

