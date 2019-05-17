// Prabhas Kumra
// CS-370
// Project 2

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

//#include "sysproc.c"
//#include "syscall.c"

int main(int argc, char * argv[])
{

    //printf(1, "This does compile\n");

    if(argc == 1){
        printf(1, "Enter another argument\n");
    }

    int ret;
    ret = strcmp(argv[1], "-s");

    char ** myArgv;
    myArgv = (char**)malloc(32*sizeof(char*));
    int i = 0;
    for(i = 0; i < 32; i++){
            myArgv[i] = (char*) malloc(128*sizeof(char));
    }

    int size;
    if(ret == 0)
        size = 2;
    else
        size = 1;

    for(i = 0; size < argc; i++){
        //myArgv[i] = argv[size];
        strcpy(myArgv[i], argv[size]);
        //printf(1, "ARGS: %s\n", myArgv[i]);
        size++;
        myArgv[i+1] = (char*)0;
    }
    


    int pid;   
    pid = fork();
    int time1 = 0;
    int time2 = 0;
 
    if(pid > 0){
        time1 = uptime();
        pid = wait();
        time2 = uptime();
    }else if(pid == 0){
        exec(myArgv[0], myArgv);
        
        printf(1, "exec failed: %s\n", myArgv[1]);
        exit();
    }else{
        printf(1, "fork error\n");
    }

    int ticks;
    ticks = time2 - time1;
    //int seconds;
    //seconds = ticks/100;

    if(ret == 0){
        printf(1, "\nReal time in seconds: %d.%d second(s)\n",  ticks/100, ticks%100);
    }else{
        printf(1, "\nReal time in ticks: %d tick(s)\n", ticks);
    }

    //printf(1, "MOD: %d\n", ticks%100);

    //printf(1, "Time1: %d\n", time1);
    //printf(1, "Time2: %d\n", time2);

    exit();
}