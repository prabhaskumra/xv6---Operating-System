// Prabhas Kumra
// Assignment #6
// CS 370

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

typedef pthread_mutex_t Mutex;
typedef pthread_cond_t Cond;

int waitingCarbon = 0, waitingLithium = 0, waitingHydrogen = 0;
int assignedCarbon = 0, assignedLithium = 0, assignedHydrogen = 0;
int globalCounter = 0;

typedef struct Semaphore {
    int value;
    int wakeups;
    Mutex *mutex;
    Cond *cond;
} Semaphore;

Semaphore  * carb;
Semaphore  * lith;
Semaphore  * hydro;
Semaphore  * globalMutex;


int notDigit(char * input);

Mutex *make_mutex();
void mutex_lock(Mutex *mutex);
void mutex_unlock(Mutex *mutex);

Cond *make_cond();
void cond_wait(Cond * cond, Mutex *mutex);
void cond_signal(Cond * cond);

Semaphore *make_semaphore(int value);
void sem_wait(Semaphore *semaphore);
void sem_signal(Semaphore *semaphore);

void * make_lithium(int thread_id);
void * make_hydrogen(int thread_id);
void * make_carbon(int thread_id);

int main(int argc, char ** argv)
{
    int bondCount;

    if(argc != 2){
        printf("Usage: ./chemistry [bondCount]\n");
        return 0;
    }
    if(notDigit(argv[1]))
        bondCount = atoi(argv[1]);
    else{
        printf("Error, bondCount should be an integer value\n");
        return 0;
    }

    int lithium = bondCount;
    int carbon = 4 * bondCount;
    int hydrogen = 9 * bondCount;

    carb = make_semaphore(0);
    lith = make_semaphore(0);
    hydro = make_semaphore(0);
    globalMutex = make_semaphore(1);

    pthread_t * lithum_threads = malloc(sizeof(pthread_t) * lithium);
    pthread_t * carbon_threads = malloc(sizeof(pthread_t) * carbon);
    pthread_t * hydrogen_threads = malloc(sizeof(pthread_t) * hydrogen);

    int i;
    for(i = 0; i < hydrogen; i++){
        pthread_create(&hydrogen_threads[i], NULL, (void *) make_hydrogen, (void *) i);
    }
    for(i = 0; i < carbon; i++){
        pthread_create(&carbon_threads[i], NULL, (void *) make_carbon, (void *) i);
    }
    for(i = 0; i < lithium; i++){
        pthread_create(&lithum_threads[i], NULL, (void *) make_lithium, (void *) i);
    }

    for(i = 0; i < hydrogen; i++){
        pthread_join(hydrogen_threads[i], NULL);
    }
    for(i = 0; i < carbon; i++){
        pthread_join(carbon_threads[i], NULL);
    }
    for(i = 0; i < lithium; i++){
        pthread_join(lithum_threads[i], NULL);
    }




    return 0;
}


int notDigit(char * input)
{
    int i;
    for(i = 0; i < strlen(input); i++)
        if(!isdigit(input[i]))
            return 0;
    return 1;
}
// Algorithm 1
Mutex *make_mutex()
{
    Mutex *mutex = malloc(sizeof(Mutex));
    int n = pthread_mutex_init(mutex, NULL);
    if(n != 0){
        perror("make_mutex failed");
        exit(1);
    }
    return mutex;
}

// Algorithm 2
void mutex_lock(Mutex *mutex)
{
    int n = pthread_mutex_lock(mutex);
    if(n != 0){
        perror("mutex_lock failed");
        exit(1);
    }
}
// Algorithm 3
void mutex_unlock(Mutex *mutex)
{
    int n = pthread_mutex_unlock(mutex);
    if(n != 0){
        perror("mutex_unlock failed");
        exit(1);
    }
}

// Algorithm 4
Cond *make_cond()
{
    Cond *cond = malloc(sizeof(Cond));
    int n = pthread_cond_init(cond,NULL);
    if(n != 0){
        perror("make_cond failed");
        exit(1);
    }
    return cond;
}
// Algorithm 5
void cond_wait(Cond * cond, Mutex *mutex)
{
    int n = pthread_cond_wait(cond, mutex);
    if(n != 0){
        perror("cond_wait failed");
        exit(1);
    }
}
// Algorithm 6
void cond_signal(Cond * cond)
{
    int n = pthread_cond_signal(cond);
    if(n != 0){
        perror("cond_signal failed");
        exit(1);
    }
}
// Algorithm 7
Semaphore *make_semaphore(int value)
{
    Semaphore *semaphore = malloc(sizeof(Semaphore));
    semaphore->value = value;
    semaphore->wakeups = 0;
    semaphore->mutex = make_mutex();
    semaphore->cond = make_cond();

    return semaphore;
}

// Algorithm 8
void sem_wait(Semaphore *semaphore)
{
    mutex_lock(semaphore->mutex);
    semaphore->value = semaphore->value - 1;

    if(semaphore->value < 0){
        do
        {
            cond_wait(semaphore->cond, semaphore->mutex);
        } while (semaphore->wakeups < 1);

        semaphore->wakeups = semaphore->wakeups - 1;
    }
    mutex_unlock(semaphore->mutex);
}

// Algorithm 9
void sem_signal(Semaphore *semaphore)
{
    mutex_lock(semaphore->mutex);
    semaphore->value = semaphore->value + 1;

    if(semaphore->value <= 0){
        semaphore->wakeups = semaphore->wakeups + 1;
        cond_signal(semaphore->cond);
    }
    mutex_unlock(semaphore->mutex);
}

//Algorithm 10
void * make_lithium(int thread_id)
{
    int id = thread_id;
    printf("\033[0mLithium atom (%d) has started\n", id);
    sem_wait(globalMutex);
    waitingLithium++;
    
  
    while(assignedLithium == 0){
        if(waitingCarbon >= 4 && waitingHydrogen >= 9 && waitingLithium >=1){
            // make N-butyllithium
            assignedCarbon += 4;
            waitingCarbon -= 4;

            assignedHydrogen += 9;
            waitingHydrogen -= 9;

            assignedLithium += 1;
            waitingLithium -= 1;

            printf("\033[0;34mN-Butyllithium (%d) made\n", globalCounter);
            globalCounter++;

            sleep(2);

            //printf("HERE Lithium\n");

            sem_signal(carb);
            sem_signal(carb);
            sem_signal(carb);
            sem_signal(carb);

            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);

            //printf("HERE Lithium2\n");
        }
        else{
            printf("\033[0;31mLithium atom (%d) waiting\n", id);
            sem_signal(globalMutex);
            sem_wait(lith);
            sem_wait(globalMutex);
        }
    }
    assignedLithium--;
    sem_signal(globalMutex);
    printf("\033[0;32mLithium atom (%d) done\n", id);
    return NULL;

}
// Algorithm 11
void * make_hydrogen(int thread_id)
{
    int id = thread_id;
    printf("\033[0mHydrogen atom (%d) has started\n", id);
    
    sem_wait(globalMutex);
    waitingHydrogen++;

    while(assignedHydrogen == 0){
        if(waitingCarbon >= 4 && waitingHydrogen >= 9 && waitingLithium >=1){

             // make N-butyllithium
            assignedCarbon += 4;
            waitingCarbon -= 4;

            assignedHydrogen += 9;
            waitingHydrogen -= 9;

            assignedLithium += 1;
            waitingLithium -= 1;

            printf("\033[0;34mN-Butyllithium (%d) made\n", globalCounter);
            globalCounter++;

            sleep(2);

            //printf("HERE Hydrogen\n");

            sem_signal(carb);
            sem_signal(carb);
            sem_signal(carb);
            sem_signal(carb);

            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);

            sem_signal(lith);

            //printf("HERE Hydrogen2\n");

        }
        else{
            printf("\033[0;31mHydrogen atom (%d) waiting\n", id);
            sem_signal(globalMutex);
            sem_wait(hydro);
            sem_wait(globalMutex);
        }
    }

    assignedHydrogen--;
    sem_signal(globalMutex);
    printf("\033[0;32mHydrogen atom (%d) done\n", id);
    return NULL;
    
}
// Algorithm 12
void * make_carbon(int thread_id)
{
    int id = thread_id;
    printf("\033[0mCarbon atom (%d) has started\n", id);
    
    sem_wait(globalMutex);
    waitingCarbon++;

    while(assignedCarbon == 0){
        if(waitingCarbon >= 4 && waitingHydrogen >= 9 && waitingLithium >=1){

             // make N-butyllithium
            assignedCarbon += 4;
            waitingCarbon -= 4;

            assignedHydrogen += 9;
            waitingHydrogen -= 9;

            assignedLithium += 1;
            waitingLithium -= 1;

            printf("\033[0;34mN-Butyllithium (%d) made\n", globalCounter);
            globalCounter++;

            sleep(2);

            //printf("HERE Carbon\n");

            sem_signal(lith);

            sem_signal(carb);
            sem_signal(carb);
            sem_signal(carb);

            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);
            sem_signal(hydro);

            //printf("HERE Carbon2\n");
                                
        }
        else{
            printf("\033[0;31mCarbon atom (%d) waiting\n", id);
            sem_signal(globalMutex);
            sem_wait(carb);
            sem_wait(globalMutex);
        }
    }    
    assignedCarbon--;
    sem_signal(globalMutex);
    printf("\033[0;32mCarbon atom (%d) done\n", id);
    return NULL;

}