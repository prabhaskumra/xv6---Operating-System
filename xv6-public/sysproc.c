#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
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
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
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


  myproc()->sz += n;

  //if(growproc(n) < 0)
  //  return -1;


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
sys_time(void)
{
  int num = 0; 

  return num;
}

int
sys_date(void)
{
  struct rtcdate *r;

  if(argptr(0, (void*)&r, sizeof(&r)) < 0)
    return -1;
  cmostime(r);
  
  return 0;
}

int
sys_IOTime(void)
{
  int times;
  if(argptr(0, (void*)&times, sizeof(&times)) < 0)
    return -1;
  
  return updateIOTime(times);

}

int
sys_wait2(void)
{
  int* creationTime; 
  int* startTime; 
  int* runTime;
  int* ioTime;
  int* endTime;

  if(argptr(0, (void*)&creationTime, sizeof(&creationTime)) < 0)
    return -1;
  if(argptr(1, (void*)&startTime, sizeof(&startTime)) < 0)
    return -1;
  if(argptr(2, (void*)&runTime, sizeof(&runTime)) < 0)
    return -1;
  if(argptr(3, (void*)&ioTime, sizeof(&ioTime)) < 0)
    return -1;
  if(argptr(4, (void*)&endTime, sizeof(&endTime)) < 0)
    return -1;

  //sint pid;
  return getProcessStats(creationTime,startTime,runTime,ioTime,endTime);

  //int waitTime;
  //waitTime = endTime - creationTime - runTime - ioTime;
 // return waitTime;
 //return 0;
  
}

int
sys_fork2(void)
{
  int tickets;

  if(argptr(0, (void*)&tickets, sizeof(&tickets)) < 0)
    return -1;

  return fork2(tickets);

}

/*
int
sys_halt(void)
{
  //outw(0x4004, 0x3400);

  // this works
  //outb(0xf4, 0x00);

  //outw(0x604, 0x2000);


  char *p = "Shutdown";
  for( ; *p; p++)
      //outb(0xf4, *p);
      outw(0x604, 0x2000);
  //outb(0x8900, *p);
    
   

//newwer versions of qemu
  //outw(0x604, 0x2000);

  return 0;
}
*/

int
sys_halt(void)
{

  char *p = "Shutdown";
  for( ; *p; p++)
    outw(0xB004, 0x2000);
    
  // asm volatile("out %0,%1" : : "a" (0xB004), "d" (0x2000));

/*
  char *p = "Shutdown";
  for( ; *p; p++)
    outb(0x8900, *p);
*/
/*
  const char *s= "Shutdown";
  for (*s; *s; ++s)
        outb (0x8900, *s);

*/

  return 0;
}

int
sys_reboot(void)
{

  return 0;
}