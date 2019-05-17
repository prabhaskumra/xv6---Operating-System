#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{

  int pid = fork();
  if(pid > 0){
    printf(1, "Inside Parent Process: child id = %d\n", pid);
    pid = wait();
    printf(1, "Hello xv6 World\n");
    printf(1, "Child %d is done executing\n", pid);
  }else if(pid == 0){
    exit();
  }else{
    printf(1, "fork error\n");
  }
  
  exit();
  //return 0;
}
