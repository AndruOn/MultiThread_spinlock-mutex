#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <semaphore.h>
#include <getopt.h>
#include <sys/time.h>
#include <errno.h>
#include "primitiv_sem.h"
#include "test_and_set.h"
#define TRUE  (1==1)
#define FALSE (!TRUE)

//declare global variable
int* mutex_read;
int* mutex_writer;
int* mutex_writelines;
int* mutex_readlines;
int* z;
prim_sem* db_write;
prim_sem* db_read;
int readcount=0;
int writecount=0;
int readlines = 1;
int writelines = 1;
int nbThreadStopReader = 0;
int nbThreadStopWriter = 0;
int nthread_writer;
int nthread_reader;


//error function : print the right error message
int error(int e, char message[]){
  printf("error: %d due to: %s", e, message);
}

//function called by writer threads 
//Stop each writer thread when writelines reach the max number of lines - number of active threads
int write_database(){
  while (rand() > RAND_MAX/10000){}
  lock_tts(mutex_writelines);
    int val = writelines++;
    if (val == 640-nthread_writer+nbThreadStopWriter)
    {
      nbThreadStopWriter++;
      val = -1;
    } 
  unlock_ts(mutex_writelines);
  return val;
}

//function called by reader threads 
//Stop each reader thread when readlines reach the max number of lines - number of active threads
int read_database(){
  while (rand() > RAND_MAX/10000)
     {}
  lock_tts(mutex_readlines);
    int val = readlines++;
    if (val == 2560-nthread_reader+nbThreadStopReader)
    {
      nbThreadStopReader++;
      val = -1;
    } 
  unlock_ts(mutex_readlines);
  return val;
  
}


//function used by writer threads
//give prority to the writer thread when it needs to access the db
void *writer(void* arg){
  while(writelines < 640){
    lock_tts(mutex_writer);
      writecount++;
      if (writecount == 1)
      {
        prim_sem_wait(db_read);
      }
    unlock_ts(mutex_writer);
    prim_sem_wait(db_write);
      // critical section, 1 writer has access to that
      int val = write_database();
      prim_sem_post(db_write);
    lock_tts(mutex_writer);
      writecount--;
      if (writecount == 0)
      {
        prim_sem_post(db_read);
      }
    unlock_ts(mutex_writer);
    if (val == -1)
    {
      break;
    }
  }
}


//function used by reader threads
////give prority to the reader thread when it needs to access the db
void *reader(void* arg){
  while(readlines <= 2560){
    lock_tts(z);
      prim_sem_wait(db_read);
        lock_tts(mutex_read);
          // critical section
          readcount++;
          if (readcount==1)
          { // first reader
            prim_sem_wait(db_write);
          }
        unlock_ts(mutex_read);
      prim_sem_post(db_read);
    unlock_ts(z);
    int val = read_database();
    lock_tts(mutex_read);
      // critical section
      readcount--;
      if(readcount==0)
      { // last reader
        prim_sem_post(db_write);
      }
    unlock_ts(mutex_read);
    if (val == -1){
      break;
    }
  }
}

int main(int argc, char const *argv[]){
  //check if number of arg is correct
  if (argc != 3){
    printf("wrong nb of args");
    return 0;
  }

  //get correct value for the argument (number of thread for reader and writer) given in command line 
  int err;
  int opt; 
  extern char *optarg;
  extern int optind, optopt;
  nthread_writer = atoi(argv[optind++]);
  nthread_reader = atoi(argv[optind]);
  pthread_t thread_writer[nthread_writer];
  pthread_t thread_reader[nthread_reader];

  //init mutex
  
  if(init_test_and_set(&mutex_read) != 0){error(-1,"mutex_init read");}
  
  if(init_test_and_set(&mutex_writer) != 0){error(-1,"mutex_init writer");}
  
  if(init_test_and_set(&mutex_writelines) != 0){error(-1,"mutex_init writelines");}
  if(init_test_and_set(&mutex_readlines) != 0){error(-1,"mutex_init readlines");}
  if(init_test_and_set(&z) != 0){error(-1,"mutex_init z");}

  //init semaphore
 
  err = prim_sem_init(&db_read, 1);
  if(err!=0) {
      error(err,"sem_init read");
  }
  err = prim_sem_init(&db_write, 1);
  if(err!=0) {
      error(err,"sem_init write");
  }

  
    
  //init thread reader
  for(int i=0;i<nthread_reader;i++) {
    err=pthread_create(&(thread_reader[i]),NULL,reader,NULL);
    if(err!=0) {
      error(err,"pthread_create reader");
    }
  }
  

  //init thread writer
  for(int i=0;i<nthread_writer;i++) {
    err=pthread_create(&(thread_writer[i]),NULL,writer,NULL);
    if(err!=0) {
      error(err,"pthread_create writer");
    }
  }

  //join thread reader
  for(int i=0;i<nthread_reader;i++) {
    err=pthread_join(thread_reader[i],NULL);
    if(err!=0) {
      error(err,"pthread_join reader");
    }
  }


  //join thread writer
  for(int i=0;i<nthread_writer;i++) {
    err=pthread_join(thread_writer[i],NULL);
    if(err!=0) {
      error(err,"pthread_join writer");
    }
  }
  //destroy mutex
  free(mutex_read);
  free(mutex_writer);
  free(mutex_writelines);
  free(mutex_readlines);
  free(z);

  //destroy semaphore writer
  err = prim_sem_destroy(db_write);
  if(err!=0) {
    error(err,"sem_destroy write");
  }

  //destroy semaphore reader
  err = prim_sem_destroy(db_read);
  if(err!=0) {
    error(err,"sem_destroy read");
  }
  return(EXIT_SUCCESS);
}
