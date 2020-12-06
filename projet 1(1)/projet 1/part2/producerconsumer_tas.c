#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <getopt.h>
#include <sys/time.h>
#include <errno.h>
#include "test_and_set.h"
#include "primitiv_sem.h"
#define TRUE  (1==1)
#define FALSE (!TRUE)
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
     
//global declaration
int* mutex;
prim_sem* empty;
prim_sem* full;
int nbItems = 0;
int remItems = 0;
int thread_producer_finished = 0;
int* buffer;
int buffersize;
int poisonkills = 0;

int index_prod = 0;
int index_cons = 0;
int nthread_producer;
int nthread_consumer;

//error function : print the right error message
int error(int e, char message[]){
  printf("error: %d due to: %s", e, message);
}

//the function called by the producers to create a new item to insert
int produce(){ return rand()%20;}

//Function that inserts a produced item or a poisonkill in the buffer,
//posionkills are usedas final tokens to tell the consumers to stop
int insert_item(int item){
    nbItems++;
    if(nbItems  >=  1025 - nthread_producer - nthread_consumer ){//the producers have inserted all the items 
        int produce_not_finished = nthread_producer - thread_producer_finished;
        int poison_kills_reamining = nthread_consumer - poisonkills;
        if(poison_kills_reamining >= produce_not_finished){ //the producers have inserted all the items 
            poisonkills++;
            buffer[index_prod++ % buffersize] = -1;
            return 0;
        }else{ //the reamining producers will insert enough poisonkills (1 per consumer) in the buffer, this producer thread can be stopped
            thread_producer_finished++; 
            return -1;
        }
    }
    buffer[index_prod++ % buffersize]=item;
    return 0;
}

//function called by thread, make producer create and insert items
//Take a pointer as argument -> here it isn't used
void* producer(void* arg)
{
    int item;
    while(TRUE)
    {
        //produce new value
        while(rand() > RAND_MAX/10000);
        item=produce();

        prim_sem_post(empty); //waiting for a empty entry in the buffer
        lock_tts(mutex);
            // critical section (inserting)
            if(insert_item(item) == -1){
                unlock_ts(mutex);
                break;
            }
        unlock_ts(mutex);
        prim_sem_post(full); //notifying that a new entry in the buffer has been filled
    }
}

//Function called by the consumers to remove an item from the buffer
int get_item(){
    remItems++;
    int item = buffer[index_cons++ % buffersize];
    return item;
}

//Function called on the removed item by the consumer
int consume(int item){
    if(item == -1){return 0;}
    while(rand() > RAND_MAX/10000);
    return 0;
}

//function called by thread, make remove items from the buffer and consume them
//Take a pointer as argument -> here it isn't used
void* consumer(void* arg){
    int item;
    while(TRUE)
    {
        prim_sem_wait(full); //waiting for a filled entry in the buffer
        lock_tts(mutex);
            // critical section (removing)
            item=get_item(item);

            if(item == -1){ // consumer found a poisonkill in the buffer, it means it has finished his work
                unlock_ts(mutex);
                prim_sem_post(empty);
                break;
            }
        unlock_ts(mutex);
        prim_sem_post(empty); //notifying that a new empty entry is available in the buffer

        consume(item);
    }
}

//Function used during the debugging to verify the validity of the inserted and removed items
//prints the buffer, the index pointers and the semaphores values
//Has to be called in a critical section 
void print_buffer(){
    printf("index_cons:%d index_prod:%d  /",index_cons%buffersize, index_prod%buffersize);
    printf("buf:[");
    for(int i=0;i<buffersize;i++) {
        printf("(%d,%d), ",i,buffer[i]);    
    }
    int f; //sem_getvalue(&full,&f);
    int e; //sem_getvalue(&empty,&e);
    printf("] s_full:%d  s_empty:%d\n",f,e);
}

int main(int argc, char const *argv[])
{
    //check if number of arg is correct
    if (argc != 3){ error(-1, "wrong number of args");}
    //get correct value for the argument (number of producers and consumers) given in command line 
    int err;
    int opt; 
    extern char *optarg;
    extern int optind, optopt;
    nthread_consumer = atoi(argv[optind++]);
    nthread_producer = atoi(argv[optind]);

    //declare threads (producers and consumers) and the buffer 
    pthread_t thread_consumer[nthread_consumer];
    pthread_t thread_producer[nthread_producer];
    buffersize = 2*max(nthread_consumer,nthread_producer);
    buffer = (int*) malloc(sizeof(int) * buffersize);
    if(buffer == NULL){error(err,"malloc buffer");}

    //init mutex and semapohre
    if(init_test_and_set(&mutex) !=0){error(-1, "init mutex error");}
    if(prim_sem_init(&empty, buffersize)!=0) {error(-1,"sem_init empty");}
    if(prim_sem_init(&full, 0)!=0) {error(-1,"sem_init full");}

    //create threads
    for(int i=0;i<nthread_producer;i++) {
        if(pthread_create(&(thread_producer[i]),NULL,producer,NULL)!=0) { error(-1,"pthread_create producer");}
    }
    for(int i=0;i<nthread_consumer;i++) {
        if(pthread_create(&(thread_consumer[i]),NULL,consumer,NULL)!=0) {error(-1,"pthread_create consumer");}
    }

    //join threads
    for(int i=0;i<nthread_producer;i++) {
        if(pthread_join(thread_producer[i],NULL)!=0) {error(-1,"pthread_join producer");}
    }
    for(int i=0;i<nthread_consumer;i++) {
        if(pthread_join(thread_consumer[i],NULL)!=0) {error(-1,"pthread_join consumer");}
    }

    //free pointers that were malloc
    free(buffer);

    //destroy mutex and semaphore
    free(mutex);
    if(prim_sem_destroy(empty)!=0) {error(-1,"sem_destroy empty");}
    if(prim_sem_destroy(full)!=0) {error(-1,"sem_destroy full");}

    return 0;
}
