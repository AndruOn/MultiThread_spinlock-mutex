#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <getopt.h>
#include <sys/time.h>
#include <errno.h>
#include <stdbool.h> 
#include "test_and_set.h"

int* lock;
//declare global variable
int nb_threads;

//function to measure the perfomances of  test and set algorithm
void* thread_fun_ts(void* arg){
    for (size_t i = 0; i < 6400/nb_threads; i++){
        lock_ts(lock);
        //printf("thread num %d, lock: %d, i: %ld\n", *(int*) arg, *lock, i);
        while(rand()> RAND_MAX/1000);
        unlock_ts(lock);
        //printf("lock: %d\n", *lock);
    }
    
}

int main(int argc, char const *argv[])
{
    //check if number of arg is correct
    if (argc != 2){
        perror("wrong number of args");
    }

    //get correct value for the argument (number of thread) given in command line
    int err;
    int opt; 
    extern char *optarg;
    extern int optind, optopt;
    nb_threads = atoi(argv[optind]);
    pthread_t threads[nb_threads]; 

    //malloc the lock variable to be able to use it in thread_fun_ts
    /*lock = malloc(sizeof(int));
    if (lock == NULL)
    {
        return -1;
    }
    
    *lock = 0;*/

   init_test_and_set(&lock);

    //init threads
    for(int i=0;i<nb_threads;i++) {
        if(pthread_create(&(threads[i]),NULL,thread_fun_ts,&i)) {perror("pthread_create");}
    }
    
    //join threads
    for(int i=0; i<nb_threads; i++) {
        if(pthread_join((threads[i]),NULL)!=0) {perror("pthread_join");}
    }



    return 0;
}

