#include <sys/types.h>
#include <errno.h>

union semun {
    int val;
    struct semid_ds *buf;
    ushort* array;
};

/*allocate a new binary semaphore*/
int binary_sem_allocate(key_t, int);

/*deallocate the binary semaphore*/
int binary_sem_deallocate(int);

/*initialize the binary semaphore with the value of 1*/
int binary_sem_init(int);

/*block until the value of the semaphore is positice, then decrement*/
int binary_sem_post(int);

/*increment the semaphore value by 1, after critical section exit*/
int binary_sem_wait(int);

/*create a semaphore with multiple values*/
int sem_allocate(key_t, int);

/*deallocate a semaphore*/
int sem_deallocate(int);

/*initialize the semaphore with a given value*/
int sem_init(int, int);

/*checks the value of the semaphore, if positive - decrement
 *return 0, if the value of semaphore was 0
*/
int sem_check(int);

/*change the value of the semaphore by 1 or -1*/
int sem_change(int, int);

int sem_down(int semid);

int sem_up(int semid);

void queueInsert(int*, int);
int queuePop(int *);
int getQueueSize(int *);
void randSleep();
int sem_getVal(int semid);


/*proc.c*/
int prodY();
int prodZ();
int cons();