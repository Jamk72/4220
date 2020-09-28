#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdint.h>

#define MAX 300

typedef struct period_info{
    struct timespec next_period;
    long period_ns;
}period_info;

struct sched_param param;

void* firstThread(void *arg);
void* secondThread(void *arg);
void* bufferThread(void *arg);
char buffer[20];

FILE *filePtr1;
FILE *filePtr2;
FILE *filePtr3;


int main(){

    filePtr1 = fopen("first.txt", "r");
    filePtr2 = fopen("second.txt", "r");
    filePtr3 = fopen("buffer.txt", "w");

    //file open check
    if(!filePtr1 || !filePtr2 || !filePtr3){
        printf("Could not execute file\n");
        return -1;
    }

    int rc;
    pthread_t thread1;
    pthread_t thread2; 
    pthread_t thread3;
    period_info pinfo;

    //creating thread1
    rc = pthread_create(&thread1, NULL, firstThread, (void*) &pinfo); 
    if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
    }
    //waiting for thread to end to continue to the next. 
    rc = pthread_join(thread1, NULL);

    //creating thread2
    rc = pthread_create(&thread2, NULL, secondThread, (void*) &pinfo); 
    if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
    }
    //waiting for thread to end to continue to the next. 
    rc = pthread_join(thread2, NULL);

    //creating thread1
    rc = pthread_create(&thread3, NULL, bufferThread, (void*) &pinfo); 
    if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
    }
    //waiting for thread to end to continue to the next. 
    rc = pthread_join(thread3, NULL);



    fclose(filePtr1);
    fclose(filePtr2);
    fclose(filePtr3);

    return 0;
}


void* firstThread(void *structArg){
    //creating the timer 
    period_info *temp;
    temp = (period_info*) structArg;
    clock_gettime(CLOCK_MONOTONIC, &(temp->next_period));

    temp->next_period.tv_nsec += 1000000;

  //  clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &(temp->next_period), NULL);

    // setting the scheduler
    param.sched_priority = 50;
    int ret;
    ret = sched_setscheduler(0, SCHED_FIFO, &param);
    if (ret == -1){
        printf("Error with setting up the scheduler\n");
        return 1;
    }


    fgets(buffer, MAX, filePtr1);
    printf("%s\n", buffer);


    pthread_exit(NULL);

}

void* secondThread(void *structArg){
    //creating the timer 
    period_info *temp;
    temp = (period_info*) structArg;
    clock_gettime(CLOCK_MONOTONIC, &(temp->next_period));

    temp->next_period.tv_nsec += 1000000;

  //  clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &(temp->next_period), NULL);

    // setting the scheduler
    param.sched_priority = 50;
    int ret;
    ret = sched_setscheduler(0, SCHED_FIFO, &param);
    if (ret == -1){
        printf("Error with setting up the scheduler\n");
        return 1;
    }


    fgets(buffer, MAX, filePtr2);
    printf("%s\n", buffer);


    pthread_exit(NULL);
    
}

void* bufferThread(void *structArg){
    //creating the timer 
    period_info *temp;
    temp = (period_info*) structArg;
    clock_gettime(CLOCK_MONOTONIC, &(temp->next_period));

    temp->next_period.tv_nsec += 1000000;

  //  clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &(temp->next_period), NULL);

    // setting the scheduler
    param.sched_priority = 50;
    int ret;
    ret = sched_setscheduler(0, SCHED_FIFO, &param);
    if (ret == -1){
        printf("Error with setting up the scheduler\n");
        return 1;
    }

    

    fputs(buffer, filePtr3);
    printf("%s\n", buffer);


    pthread_exit(NULL);

}