// Prabhas Kumra
// Project 5
// CS-370

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdatomic.h>


int globalCounter = 0;
unsigned long globalArr[1000000];
unsigned long maxValue = 0;
int threadCount = 0;
int visualization = 0;
int lockType = 0;
int lpCount = 0;

#define LOCKED 1

atomic_int tasLock = 0;
atomic_int tasLk = 0;
/*
int TestAndSet( int lockptr)
{
    int oldValue;

    oldValue = lockptr;
    lockptr = LOCKED;

    return oldValue;
}

int testandset(int * flag)
{
    int old = *flag;
    *flag = 1;
    return old;
}

struct tasLk
{
    atomic_int * held;
};
struct tasLk * tLock;

void initializeTAS()
{
    atomic_fetch_or(&tLock->held, 1);
}
void acquire ()
{
    while(testandset(&tLock->held));
}
void release ()
{
    atomic_fetch_or(&tLock->held, 0);
}
*/

struct args
{
    int id;
    unsigned long num;
};


struct node
{
    unsigned long data;
    struct node * link;
};

struct node * head = NULL;
struct node * tail = NULL;
    

struct peterson2ThdLock
{
   // atomic_int * level;
   // atomic_int * victim;
    int * level;
    int * victim;
};

struct peterson2ThdLock * myLock;

void initializeLock( struct peterson2ThdLock * lock)
{
    //myLock->level = (atomic_int*) malloc(threadCount * sizeof(atomic_int));
    myLock->level = (int*) malloc(threadCount * sizeof(int));
    //myLock->victim = (atomic_int*) malloc(threadCount-1 * sizeof(atomic_int));
    myLock->victim = (int*) malloc(threadCount-1 * sizeof(int));
}

void lock(int threadID)
{
    //atomic_store(&lock->flag[threadID],1);
    //atomic_store(&lock->victim, threadID);
    //while(atomic_load(&lock->flag[(threadID+1)%2]) == 1 && atomic_load(&lock->victim) == threadID)
    //{

    //}
    //atomic_int  * me;
    //atomic_store(&me, threadID);
    int me = threadID;

    int i;
    int k = 0;
    for(i = 1; i < threadCount; i++){
        myLock->level[me] = i;
        myLock->victim[i] = me;
        
        // Search for another thread in a level greater than or equal to me
        for(int k = 0; k < threadCount; k++) {
            if(k == me)
                continue;

        }

    }

}     

void unlock(int threadID)
{
    //atomic_store(&lock->flag[threadID],0);
    myLock->level[threadID] = -1;
}

int notDigit(char * input)
{
    int i;
    int length = strlen(input);
    for(i = 0; i < length; i++){
        if (!isdigit(input[i])){
            return 0;
        }
    }
    return 1;
}

int  sophiePrimes(unsigned long number)
{
    unsigned long i = 0;
    for(i = 2; i <= number/2; i++){
        if(number % i == 0){
            return 0;
        }
    }
    for(i = 2; i <= number/2; i++){
        if( ((2*number) + 1) % i == 0 ){
            return 0;
        }
    }

    /*

    if(head == NULL){
        head = (struct node*)malloc(sizeof(struct node));
        head->data = number;
        head->link = NULL;
        tail = head;
    }
    else{
        struct node * temp = (struct node*)malloc(sizeof(struct node));
        temp->data = number;
        temp->link = NULL;
        tail->link = temp;
        tail = temp;
    }
    */

    return 1;
}

void PetersonAlgorithm(int id)
{
    //struct peterson2ThdLock * lk;
    //initializeLock (lk);
    //lock( id);
    unsigned long i;
        for(i = 2; i <= maxValue; i++){
            if(sophiePrimes(i))
            {
                globalArr[globalCounter] = i;
                globalCounter++;
            }
    }
    //unlock(id);
}

void * testAndset(unsigned long number)
{
        printf("TAS Lock\n");
        unsigned long i;

        for(i = 2; i <= maxValue; i++){

            //while (atomic_fetch_or(&tasLock, 1)==1){
			    
		    //}

            //atomic_store(&tasLock, 0);

            //if(lpCount > maxValue)
              //   break;

            if(sophiePrimes(i))
            {
                
                //while(TestAndSet(&tasLock) == 1){
                    //acquire();
            

                while(1) {
                    // Keep spinning if lock is acquired by someone
                    while(atomic_load(&tasLk));

                    // Attempt to acquire lock
                    // If atomic_fetch_or returns 0(false), acquired lock so break
                    // Else keep looping
                    if(!atomic_fetch_or(&tasLk, 1)) {
                        break;
                    }
                    
                }
                globalArr[globalCounter] = i;
                globalCounter++;

                atomic_store(&tasLk, 0);
            }
            //lpCount++;
    }
    pthread_exit((void**)NULL);
}

void waitFree(unsigned long number)
{

    unsigned long i;
        for(i = 2; i <= maxValue; i++){
            if(sophiePrimes(i))
            {
                globalArr[globalCounter] = i;
                globalCounter++;
            }
    }

}

void noLock(unsigned long number)
{

    unsigned long i;
        for(i = 2; i <= maxValue; i++){
            if(sophiePrimes(i))
            {
            }
    }

}




int main(int argc, char ** argv)
{
    /* unsigned long maxValue = 0;
    int threadCount = 0;
    int visualization = 0;
    int lockType = 0; */

    if(argc < 5){
        printf("Error, too few arguments\nUsage: ./prime.c [threadCount] [maxvalue] [lockType] [visualization]\n");
        return 0;
    }
    if(argc > 5){
        printf("Error, too many arguments\nUsage: ./prime.c [threadCount] [maxvalue] [lockType] [visualization]\n");
        return 0;
    }

    int isNot = notDigit(argv[1]);
    if(isNot == 0){
        printf("Error, wrong value for thread count\n");
        return 0;
    }
    threadCount = atoi(argv[1]);

    isNot = notDigit(argv[2]);
    if(isNot == 0){
        printf("Error, wrong value for max value\n");
        return 0;
    }
    maxValue = atol(argv[2]);

    isNot = notDigit(argv[3]);
    if(isNot == 0){
        printf("Error, wrong value for lock type\n");
        return 0;
    }
    lockType = atoi(argv[3]);
    if(lockType > 3 || lockType < 0){
        printf("lockType should be between 0 and 3 inclusive\n");
        return 0;
    }


    isNot = notDigit(argv[4]);
    if(isNot == 0){
        printf("Error, wrong value for visualization\n");
        return 0;
    }
    visualization = atoi(argv[4]);
    if(visualization > 1 || visualization < 0){
        printf("Visualization should be either 0 or 1\n");
        return 0;
    }

    //head = (struct node*)malloc(sizeof(struct node));
    //tail = (struct node*)malloc(sizeof(struct node));
    //head = tail;
    pthread_t * threads = malloc(sizeof(pthread_t) * threadCount);
    int i;

    double start, end;
    double total;

    
    //initializeLock(myLock);
    //initializeTAS();
    //atomic_store(&tasLock, 0);

    start = (double)(clock()) /CLOCKS_PER_SEC;
    for(i = 0; i < threadCount; i++){

        if(lockType == 0)
            pthread_create(&threads[i], NULL, (void *) noLock, (void *) i);

        if(lockType == 1)
            pthread_create(&threads[i], NULL, (void *) PetersonAlgorithm, (void *) i);
        
        else if(lockType == 2)
            pthread_create(&threads[i], NULL, (void *) testAndset, (void *) maxValue);

        else if(lockType == 3)
            pthread_create(&threads[i], NULL, (void *) waitFree, (void *) maxValue);
    }

    for(i = 0; i < threadCount; i++){
        pthread_join(threads[i], NULL);
    }

    

    end = (double)(clock())/CLOCKS_PER_SEC;
    total = (double)(end - start);


    /*unsigned long i;
    for(i = 2; i <= maxValue; i++){
        if(sophiePrimes(i))
        {
        }
    }*/
    //PetersonAlgorithm(maxValue);
    printf("====Results====\n===============\n");
    printf("The primes are:\n");
    i = 0;
    
    /*struct node * temp = head;
    while(temp != NULL){
        printf("%lu ", temp->data);
        if(i == 0){
        }
        else if( i % 12 == 0)
            printf("\n");
        temp = temp->link;
        i++;
    }*/

    for(int i = 0; i < globalCounter; i++){
        printf("%lu ", globalArr[i]);
        if(i == 0){
        }
        else if( i % 12 == 0)
            printf("\n");

    }

    printf("\nThere are %d Germain primes between 1 and %lu \n", globalCounter, maxValue);
    printf("Thread Count %d\n", threadCount);
    printf("Lock Type: ");
    if(lockType == 0)
        printf("No Mutexes\n");
    else if(lockType == 1)
        printf("Peterson's Lock\n");
    else if(lockType == 2)
        printf("TAS Lock\n");
    else if(lockType == 3)
        printf("Wait Free\n");

    printf("Time taken %f seconds\n", total);

    pthread_exit(NULL);

    return 0;
}