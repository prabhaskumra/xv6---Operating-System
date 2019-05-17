// Prabhas Kumra
// CS-370
// Project 4.1

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{

    const int CPU_ITERATIONS = 10000000;
    //int n, cpuBurts, ioBursts;
    //int arrivalTime, numCPUBursts;

    char * fname = argv[1];
    int flength = strlen(fname);

    if(fname[flength-4] != '.'){
        printf(1, "Error, wrong file. Please enter a .txt file\n");
        exit();
    }
    if(fname[flength-3] != 't'){
        printf(1, "Error, wrong file. Please enter a .txt file\n");
        exit();
    }
    if(fname[flength-2] != 'x'){
        printf(1, "Error, wrong file. Please enter a .txt file\n");
        exit();
    }
    if(fname[flength-1] != 't'){
        printf(1, "Error, wrong file. Please enter a .txt file\n");
        exit();
    }

    int fd = open(argv[1], O_RDONLY);

    if(fd < 0){
        printf(1, "Error opening the file\n");
        exit();
    }

    char buf[512];
    int chars;

    for(;;){
        chars = read(fd, buf, sizeof(buf)); 

        if (chars == 0) {
            break;
        }
        if(chars < sizeof(buf))
            break;
        if(chars < 0){
            printf(1, "Error reading the file\n");
            exit();
        }        
    }
    close(fd);
    char * nProcesses = (char *)malloc(128 * sizeof(char));
    char * nums = (char *)malloc(128 * sizeof(char));
    
    int n;
    int i = 0;
    while(buf[i] != '\n'){
        nProcesses[i] = buf[i];
        i++;
    }
    i++;
    n = atoi(nProcesses);
    int arrivalTime[n];
    int tickets[n];
    int r = 64, c = 10;
    //int *myArray = (int *)malloc(r * c * sizeof(int));
    int myArray[r][c];

    int x, y;

    for(x = 0; x < r; x++)
        for(y = 0; y < c; y++)
            myArray[x][y] = -1;

    //printf(1, "I %d \n", i);
    int k =0, count, spaceCount;

    for(k=0; k < n; k++){

        spaceCount = 0;
        count = 0;
        memset(nums, 0, 128*sizeof(char));

        while(buf[i] != '\n'){
            

            if(buf[i] == ' '){
                spaceCount++;

                if(spaceCount == 1){
                    arrivalTime[k] = atoi(nums);
                    memset(nums, 0, 128*sizeof(char));
                    count = -1;
                }

                else if(spaceCount == 2){
                    tickets[k] = atoi(nums);
                    //myArray[k][spaceCount - 2] = atoi(nums);
                    //temp = atoi(nums);
                    memset(nums, 0, 128*sizeof(char));
                    count = -1;
                }

                else if(spaceCount == 3){
                    //tickets[k] = atoi(nums);
                    myArray[k][spaceCount - 3] = atoi(nums);
                    //temp = atoi(nums);
                    memset(nums, 0, 128*sizeof(char));
                    count = -1;
                }

                else if(spaceCount > 3){
                    myArray[k][spaceCount-3] = atoi(nums);
                    memset(nums, 0, 128*sizeof(char));
                    count = -1;
                }
            } 
            else
                nums[count] = buf[i];

        i++;
        count++;
        }
        myArray[k][spaceCount-2] = atoi(nums);

        i++;
    }

/*
    for(x = 0; x < r; x++){
        for(y = 0; y < c; y++){
            printf(1, "%d ", myArray[x][y]);   
        }
        printf(1, "\n");
   }
   

   
   for(x = 0; x< n; x++)
    printf(1, "ARRIVAL TIMES %d \n", arrivalTime[x]);   
*/
   /* for(x = 0; x< n; x++)
    printf(1, "ARRIVAL TIMES %d \n", arrivalTime[x]);
    for(x = 0; x< n; x++)
    printf(1, "Tickets %d \n", tickets[x]);*/

    

    int currentTime, pid;
    int sTime = uptime();
    int counter;
    int pids[n];
    int creationTime, startTime, runTime, ioTime, endTime, waitTime[n];
    int tTime[n], rTime[n];
    int avgtTime=0, avgrTime=0;
    //int y;
    
    for(i=0; i<n; i++){
        currentTime = uptime();
        if(arrivalTime[i] > (currentTime - sTime)){
            sleep(arrivalTime[i] - (currentTime - sTime));
        }

        pid = fork2(tickets[i]);
        if(pid == 0){
            // child process algorithm
            counter = 0;
            pid = getpid();

            for(x = 0; x < myArray[i][1]*CPU_ITERATIONS; x++){
                counter++;
            }

            /*int b=2, sum = 0;
            while(myArray[i][b] != -1){
                sum += myArray[i][b];
                //printf(1, "SUM %d \n", sum);
                b++;
            }*/
            //printf(1, "SUM %d \n", sum);
            
            x = 2; 
            int z = 3, a;
            //while(sum[i] > 0){
            for(a = 0; a < myArray[i][0]-1; a++){

                for( y = 0; y < myArray[i][x]; y++){
                    printf(1, "Child %d prints for the %d time\n", pid, y+1);
                    //sum[i]--;
                }
                //sum[i]--;
                for(y = 0; y < myArray[i][z]*CPU_ITERATIONS; y++){
                    counter++;
                    //sum[i]--;
                }
                //sum[i]--;
                x+=2;
                z+=2;

            }
            exit();
        }
        
    }

    int cTime[n], stTime[n], eTime[n], RTime[n], iTime[n];

    for(i = 0; i < n; i++){
        pids[i] = wait2(&creationTime, &startTime, &runTime, &ioTime, &endTime);
        cTime[i] = creationTime;
        stTime[i] = startTime;
        eTime[i] = endTime;
        RTime[i] = runTime;
        iTime[i] = ioTime;

        tTime[i] = endTime - creationTime;
        rTime[i] = startTime - creationTime;
        waitTime[i] = endTime - creationTime - runTime - ioTime;
    }
    

    for(i = 0; i < n; i++){
        printf(1, "Child %d: ctime - %d - stime - %d - etime - %d\n     iotime - %d - rtime - %d - wtime - %d\n      turnaround time - %d - response time - %d\n", pids[i], cTime[i], stTime[i], eTime[i], iTime[i], rTime[i], waitTime[i], tTime[i], RTime[i] );

        //printf(1, " PID %d \n", pids[i]);
        avgrTime += rTime[i];
        avgtTime += tTime[i];
    }
    avgrTime = avgrTime/n;
    avgtTime = avgtTime/n;
    printf(1, "Avg TAT: %d \nAvg Response: %d \n", avgtTime, avgrTime);


    exit();
}