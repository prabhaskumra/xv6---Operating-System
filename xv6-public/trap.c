#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"

extern int mappages(pde_t *pgdir, void *va, uint size, uint pa, int perm);

// Interrupt descriptor table (shared by all CPUs).
struct gatedesc idt[256];
extern uint vectors[];  // in vectors.S: array of 256 entry pointers
struct spinlock tickslock;
uint ticks;

void
tvinit(void)
{
  int i;

  for(i = 0; i < 256; i++)
    SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);
  SETGATE(idt[T_SYSCALL], 1, SEG_KCODE<<3, vectors[T_SYSCALL], DPL_USER);

  initlock(&tickslock, "time");
}

void
idtinit(void)
{
  lidt(idt, sizeof(idt));
}

//PAGEBREAK: 41
void
trap(struct trapframe *tf)
{
  if(tf->trapno == T_SYSCALL){
    if(myproc()->killed)
      exit();
    myproc()->tf = tf;
    syscall();
    if(myproc()->killed)
      exit();
    return;
  }



  ////////////////////////////////////////////////////////////
/*
  if(tf->trapno == 14){

    char *mem;
    uint a;
    a = PGROUNDDOWN(rcr2());

    for(;a < myproc()->sz; a += PGSIZE){
      mem = kalloc();
      if(mem == 0){
        cprintf("allocuvm out of memory\n");
        //deallocuvm(proc->pgdir, newsz, oldsz);
        return;
      }
      memset(mem, 0, PGSIZE);

      mappages(myproc()->pgdir, (char*)a, PGSIZE, V2P(mem), PTE_W|PTE_U);
      //if(mappages(myproc()->pgdir, (char*)a, PGSIZE, V2P(mem), PTE_W|PTE_U) < 0){
        //cprintf("allocuvm out of memory (2)\n");
        //deallocuvm(pgdir, newsz, oldsz);
        //kfree(mem);
        return;
      }
    }

    */
  //}




////////////////////////////////////////////////////////////




  switch(tf->trapno){
  case T_IRQ0 + IRQ_TIMER:
    if(cpuid() == 0){

      acquire(&tickslock);
      ticks++;
      wakeup(&ticks);
      release(&tickslock);
    }
    //struct proc *p = myproc();
      if(myproc() && (tf->cs & 3) == 3 ){
        if(myproc() && myproc()->state == RUNNING)
          myproc()->runTime++;
      }
        

    lapiceoi();
    //myproc()->runTime++;
    break;
  case T_IRQ0 + IRQ_IDE:
    ideintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE+1:
    // Bochs generates spurious IDE1 interrupts.
    break;
  case T_IRQ0 + IRQ_KBD:
    kbdintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_COM1:
    uartintr();
    lapiceoi();
    break;
  case T_IRQ0 + 7:
  case T_IRQ0 + IRQ_SPURIOUS:
    cprintf("cpu%d: spurious interrupt at %x:%x\n",
            cpuid(), tf->cs, tf->eip);
    lapiceoi();
    break;

  //PAGEBREAK: 13
  default:
    if(myproc() == 0 || (tf->cs&3) == 0){
      // In kernel, it must be our mistake.
      cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
              tf->trapno, cpuid(), tf->eip, rcr2());
      panic("trap");
    }
    // In user space, assume process misbehaved.

  ////////////////////////////////////////////////////////////

  if(tf->trapno == 14){

    char *mem;
    uint a;

    //a = PGROUNDUP(rcr2());
    //a = rcr2();
    a = PGROUNDDOWN(rcr2());

    for(;a < myproc()->sz; a += PGSIZE){
      mem = kalloc();
      //if(mem == 0){
     //   cprintf("allocuvm out of memory\n");
        //deallocuvm(proc->pgdir, newsz, oldsz);
     //   return;
     // }
      memset(mem, 0, PGSIZE);

      mappages(myproc()->pgdir, (char*)a, PGSIZE, V2P(mem), PTE_W|PTE_U);
      //if(mappages(myproc()->pgdir, (char*)a, PGSIZE, V2P(mem), PTE_W|PTE_U) < 0){
        //cprintf("allocuvm out of memory (2)\n");
        //deallocuvm(pgdir, newsz, oldsz);
        //kfree(mem);
        return;
      }
    }

////////////////////////////////////////////////////////////


    cprintf("pid %d %s: trap %d err %d on cpu %d "
            "eip 0x%x addr 0x%x--kill proc\n",
            myproc()->pid, myproc()->name, tf->trapno,
            tf->err, cpuid(), tf->eip, rcr2());

    if(tf->trapno == 0)
      cprintf("Divide Error\n");

    if(tf->trapno == 1)
      cprintf("Debug Exception");
    
    if(tf->trapno == 2)
      cprintf("Non-Maskable Interrupt\n");

    if(tf->trapno == 3)
      cprintf("Breakpoint\n");
    
    if(tf->trapno == 4)
      cprintf("Overflow\n");

    if(tf->trapno == 5)
      cprintf("Bounds Check\n");
    
    if(tf->trapno == 6)
      cprintf("Illegal Opcode\n");

    if(tf->trapno == 7)
      cprintf("Device Not Available\n");
    
    if(tf->trapno == 8)
      cprintf("Double Fault\n");

    if(tf->trapno == 10)
      cprintf("Invalid Task Switch Segment\n");
    
    if(tf->trapno == 11)
      cprintf("Segment Not Present\n");

    if(tf->trapno == 12)
      cprintf("Stack Exception\n");
    
    if(tf->trapno == 13)
      cprintf("General Protection Fault\n");

    if(tf->trapno == 14)
      cprintf("Page Fault\n");
    
    if(tf->trapno == 16)
      cprintf("Floating Point Error\n");

    if(tf->trapno == 17)
      cprintf("Aligment Check\n");
    
    if(tf->trapno == 18)
      cprintf("Machine Check\n");

    if(tf->trapno == 18)
      cprintf("SIMD Floating Point Error\n");
    
    myproc()->killed = 1;
  }

  // Force process exit if it has been killed and is in user space.
  // (If it is still executing in the kernel, let it keep running
  // until it gets to the regular system call return.)
  if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
    exit();

  // Force process to give up CPU on clock tick.
  // If interrupts were on while locks held, would need to check nlock.
  if(myproc() && myproc()->state == RUNNING &&
     tf->trapno == T_IRQ0+IRQ_TIMER){

      
      yield();

     }
   

  // Check if the process has been killed since we yielded
  if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
    exit();
}


