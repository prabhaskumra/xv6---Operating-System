// Prabhas Kumra
// CS-370
// Project 2

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


int main(int argc, char * argv[])
{

    if(argc == 1){
        printf(1, "Another argument expected\n");
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
        strcpy(myArgv[i], argv[size]);
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

    if(ret == 0){
        printf(1, "\nReal time in seconds: %d.%d second(s)\n\n",  ticks/100, ticks%100);
    }else{
        printf(1, "\nReal time in ticks: %d tick(s)\n\n", ticks);
    }

    exit();
}