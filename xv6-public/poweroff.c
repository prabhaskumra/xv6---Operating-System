#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    printf(1, "powering off the pc");
    
    int i;

    for(i = 0; i < 7; i++){
        sleep(15);
        printf(1, ".");
    }
    printf(1, ".\n");
    halt();
    exit();
}

