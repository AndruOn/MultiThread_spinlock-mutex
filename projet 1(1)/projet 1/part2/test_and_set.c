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


int init_test_and_set(int** arg){
    *arg = malloc(sizeof(int));
    if (arg == NULL)
    {
        return -1;
    }
    
    **arg = 0;
    return 0;
}

//take a pointer of int and an int as param and swap their values
//return the (potentially) new value of the int param
int test_and_set(volatile int* lock, int lock_val){
    int* add_lock_val = &lock_val;
    asm volatile ("\
            xchgl %0, %1;"
            :"+m" (*lock), "+q" (lock_val) //output
    );
    return lock_val;
}

//take a pointer of int as param and run test and set with its value and 1
//lock thread when test and set return 1 (when lock value is already 1)
int lock_ts(int *lock){
    while (test_and_set(lock, 1)){
        
    };
}


//take a pointer of int as param and run test and set with its value and 0
//unlock a thread
void unlock_ts(volatile int* lock){
    test_and_set(lock, 0);
}

//take a pointer of int as param and run test and set with its value and 1 
//then read the value and wait for it to change
//lock thread when test and set return 1 (when lock value is already 1)
int lock_tts(volatile int *lock){
    while (test_and_set(lock, 1)){
        while (*lock){
        };
    };
}




