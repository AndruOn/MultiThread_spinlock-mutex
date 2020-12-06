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

#define true 1
#define NBOFCYCLES 1000000

//global declaration
int numberOfPhilo;
int* baguette;
__thread int cycles = 0;

//error function : print the right error message
int error(int e, char message[]){
  printf("error: %d due to: %s", e, message);
}


//"mange" function, does nothing
void mange(int id) { 
    //printf("Philosophe [%d] mange\n",id); 
}


//function called by thread, make philosophe eat and think. 
//Take a pointer as argument -> the list of the mutex
void* philosophe ( void* arg ) {
    int *id=(int *) arg; 
    int left = *id; 
    int right = (left + 1) % numberOfPhilo; 
    if (numberOfPhilo == 1)
    {
        right = 1;
    }
    
    while(cycles<NBOFCYCLES) {
        // philosophe pense 
        //printf("Philosophe [%d] pense\n",*id);
        
        if(left<right) {
            lock_tts(&baguette[left]); 
            //printf("Philosophe [%d] possède baguette gauche [%d]\n",*id,left);
            lock_tts(&baguette[right]);
            //printf("Philosophe [%d] possède baguette droite [%d]\n",*id,right);
        } else {
            lock_tts(&baguette[right]);
            //printf("Philosophe [%d] possède baguette droite [%d]\n",*id,right); 
            lock_tts(&baguette[left]);
            //printf("Philosophe [%d] possède baguette gauche [%d]\n",*id,left);
        } 
        mange(*id);
        //print_lock_val();
        cycles++;
        //print_lock_val();
        unlock_ts(&baguette[left]); 
        //printf("Philosophe [%d] a libéré baguette gauche [%d]\n",*id,left); 
        unlock_ts(&baguette[right]);
        //print_lock_val();
        //printf("Philosophe [%d] a libéré baguette droite [%d]\n",*id,right);
        
        
        //if(cycles%1000==0){printf("Philosophe [%d] a fini [%d] cycles\n",*id,cycles);}
    } 
    return (NULL);
}


int main(int argc, char const *argv[])
{
    //check if number of arg is correct
    if (argc != 2){
        error(-1, "wrong number of args");
    }
    //get correct value for the argument (number of thread) given in command line 
    int err;
    int opt; 
    extern char *optarg;
    extern int optind, optopt;
    numberOfPhilo = atoi(argv[optind]);
    int numberOfBaguette = numberOfPhilo;
    if(numberOfPhilo==1){numberOfBaguette = 2;}
    
    //declare thread (philosophe) and mutex (baguette)
    pthread_t phil[numberOfPhilo]; 
    baguette = malloc(sizeof(int) * numberOfBaguette);
    if(baguette == NULL){error(err,"malloc baguette");}

    //create list of baguette needed in the philosophe function
    int* index = malloc(sizeof(int) * numberOfBaguette);
    if(index == NULL){error(err,"malloc index");}
    if (numberOfPhilo == 1){
    }
    {
        index[1] = 1;
    }


    //init mutex
    for (size_t i = 0; i < numberOfBaguette; i++){
        baguette[i] = 0;
    }

    //create thread
    for(int i=0;i<numberOfPhilo;i++) {
        index[i]=i;
        if(pthread_create(&(phil[i]),NULL,philosophe,&(index[i]))!=0) { error(err,"pthread_create");}
    }
    
    
    
    //join thread
    for(int i=0; i<numberOfPhilo; i++) {
        if(pthread_join((phil[i]),NULL)!=0) {error(err, "pthread_join");}
    }


    // free pointeur that was malloc
    free(baguette);
    free(index);

    return 0;
}
