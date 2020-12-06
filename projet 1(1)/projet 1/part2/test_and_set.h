#ifndef HEADER_FILE_TAS
#define HEADER_FILE_TAS

/***************************
 * test and set implem
 * *************************/
int test_and_set(volatile int*, int);
int lock_ts(int *);
void unlock_ts(volatile int*);
int init_test_and_set(int**);


/***************************
 * test and set implem
 * *************************/
int lock_tts(volatile int *);

#endif