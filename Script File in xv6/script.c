// Prabhas Kumra
// CS-370
// Project 1

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
    
    int num;

    if (argc == 3) {

        num = atoi(argv[2]);

    }else if (argc == 2) {
        num = 0;
    }else
    {
        printf(1, "Command Line Error\n");
        exit();
    }

    char * fname = argv[1];
    int flength = strlen(fname);

    if(fname[flength-3] != '.'){
        printf(1, "Error, wrong file. Please enter a .sh file\n");
        exit();
    }
    if(fname[flength-2] != 's'){
        printf(1, "Error, wrong file. Please enter a .sh file\n");
        exit();
    }
    if(fname[flength-1] != 'h'){
        printf(1, "Error, wrong file. Please enter a .sh file\n");
        exit();
    }

    int fd = open(argv[1], O_RDONLY);

    if(fd < 0){
        printf(1, "Error opening the file\n");
        exit();
    }

    char buf[512];
    int n;

    for(;;){
        n = read(fd, buf, sizeof(buf)); 

        if (n == 0) {
            break;
        }
        if(n < sizeof(buf))
            break;
        if(n < 0){
            printf(1, "Error reading the file\n");
            exit();
        }        
    }

    close(fd);

    int x = 0;
    if(num == 1){ 
        for(x = 0; x < n; x++){
            printf(1, "%c", buf[x]);
        }
    }

    int i = 0, count = 0;

    int pid;

    while(i < n){

        count = 0;
        char * strname = (char *)malloc(128 * sizeof(char));
        char ** myArgv;

        myArgv = (char**)malloc(32*sizeof(char*));
        int k = 0;
        for(k = 0; k < 32; k++)
        {
            myArgv[k] = (char*) malloc(128*sizeof(char));
        }
        while(buf[i] != '\n'){

            int q = 0;
            while(buf[i] != ' '){
                
                strname[q] = buf[i];
                i++;
                q++;
                if(i>=n )
                break;
                if(buf[i] == '\n'){
                    break;
                }
            }
           
            strcpy(myArgv[count], strname);
            memset(strname, 0, 128*sizeof(char));

            if(buf[i] == '\n'){
                break;
                }
            
            count++;
            i++;
            
            if(i >= n)
                break;
        }
        myArgv[count+1] = (char*)0;
        int q;
        
        pid = fork();
 
        if(pid > 0){
             pid = wait();
        }else if(pid == 0){
            exec(myArgv[0], myArgv);
            printf(1, "exec failed: %s\n", myArgv[0]);
            exit();
        }else{
            printf(1, "fork error\n");
        }
        i++;
        for(q = 0; q < 32; q++)
        {
            memset(myArgv[q], 0, sizeof(myArgv[q]));
        }
    }
  
    exit();
}