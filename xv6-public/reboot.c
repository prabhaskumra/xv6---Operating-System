#include "types.h"
#include "stat.h"
#include "user.h"


int main(void)
{
   
   /* uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
    halt();
    */
    reboot();
    exit();
}



