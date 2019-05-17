#include "types.h"
#include "stat.h"
#include "user.h"


int main(int agrc, char ** argv)
{
    printf(1, "halting the pc\n");
    
    halt();
    exit();
}