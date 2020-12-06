#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <semaphore.h>
#include <getopt.h>
#include <sys/time.h>
#include <errno.h>
#define TRUE  (1==1)
#define FALSE (!TRUE)

//declare global variable
pthread_mutex_t mutex_read;
pthread_mutex_t mutex_writer;
pthread_mutex_t mutex_writelines;
pthread_mutex_t mutex_readlines;
pthread_mutex_t z;
sem_t db_write;
sem_t db_read;
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
  while (rand() > RAND_MAX/10000)
     {}
  pthread_mutex_lock(&mutex_writelines);
    int val = writelines++;
    if (val == 640-nthread_writer+nbThreadStopWriter)
    {
      nbThreadStopWriter++;
      val = -1;
    } 
  pthread_mutex_unlock(&mutex_writelines);
  return val;
}

//function called by reader threads 
//Stop each reader thread when readlines reach the max number of lines - number of active threads
int read_database(){
  while (rand() > RAND_MAX/10000)
     {}
  pthread_mutex_lock(&mutex_readlines);
    int val = readlines++;
    if (val == 2560-nthread_reader+nbThreadStopReader)
    {
      nbThreadStopReader++;
      val = -1;
    } 
  pthread_mutex_unlock(&mutex_readlines);
  return val;
  
}


//function used by writer threads
//give prority to the writer thread when it needs to access the db
void *writer(void* arg){
  while(TRUE){
    pthread_mutex_lock(&mutex_writer);
      writecount++;
      if (writecount == 1)
      {
        sem_wait(&db_read);
      }
    pthread_mutex_unlock(&mutex_writer);
    sem_wait(&db_write);
      // critical section, 1 writer has access to that
      int val = write_database();
      sem_post(&db_write);
    pthread_mutex_lock(&mutex_writer);
      writecount--;
      if (writecount == 0)
      {
        sem_post(&db_read);
      }
    pthread_mutex_unlock(&mutex_writer);
    if (val == -1)
    {
      break;
    }
  }
}


//function used by reader threads
////give prority to the reader thread when it needs to access the db
void *reader(void* arg){
  while(TRUE){
    pthread_mutex_lock(&z);
      sem_wait(&db_read);
        pthread_mutex_lock(&mutex_read);
          // critical section
          readcount++;
          if (readcount==1)
          { // first reader
            sem_wait(&db_write);
          }
        pthread_mutex_unlock(&mutex_read);
      sem_post(&db_read);
    pthread_mutex_unlock(&z);
    int val = read_database();
    pthread_mutex_lock(&mutex_read);
      // critical section
      readcount--;
      if(readcount==0)
      { // last reader
        sem_post(&db_write);
      }
    pthread_mutex_unlock(&mutex_read);
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
  if(pthread_mutex_init(&mutex_read, NULL) != 0){error(-1,"mutex_init read");}
  if(pthread_mutex_init(&mutex_writer, NULL) != 0){error(-1,"mutex_init writer");}
  if(pthread_mutex_init(&mutex_writelines, NULL) != 0){error(-1,"mutex_init writelines");}
  if(pthread_mutex_init(&mutex_readlines, NULL) != 0){error(-1,"mutex_init readlines");}
  if(pthread_mutex_init(&z, NULL) != 0){error(-1,"mutex_init z");}

  //init semaphore
  err = sem_init(&db_read, 0, 1);
  if(err!=0) {
      error(err,"sem_init read");
  }
  err = sem_init(&db_write, 0, 1);
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

  //join thread reade
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
  if(pthread_mutex_destroy(&mutex_writer) != 0){error(-1, "destroy mutex writer");}
  if(pthread_mutex_destroy(&mutex_read) != 0){error(-1, "destroy mutex read");}
  if(pthread_mutex_destroy(&mutex_writelines) != 0){error(-1, "destroy mutex writelines");}
  if(pthread_mutex_destroy(&mutex_readlines) != 0){error(-1, "destroy mutex readlines");}
  if(pthread_mutex_destroy(&z) != 0){error(-1, "destroy mutex z");}

  //destroy semaphore writer
  err = sem_destroy(&db_write);
  if(err!=0) {
    error(err,"sem_destroy write");
  }

  //destroy semaphore reader
  err = sem_destroy(&db_read);
  if(err!=0) {
    error(err,"sem_destroy read");
  }
  return(EXIT_SUCCESS);
}
