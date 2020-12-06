#ifndef HEADER_FILE_PRIM
#define HEADER_FILE_PRIM

typedef struct primitive_sem{
    int val;
    int* lock;
}prim_sem;


int prim_sem_init(prim_sem**, int);
int prim_sem_destroy(prim_sem*);
int prim_sem_wait(prim_sem*);
int prim_sem_post(prim_sem*);
void print_sem(prim_sem*);


#endif