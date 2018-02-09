#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include "keys.h"
#include "sem.h"

int Y_id, Z_id, YZ_id;
int semY_id, semZ_id, Yfree_id, Yfull_id, Zfree_id, Zfull_id;
int try_bufY, try_bufZ, try_bufYZ;
int M, N, P, R, S, T, seg_size;
int *BY, *BZ, *YZ;
pid_t pid;
pid_t fpid;

void makeforks(int n) {
	int i;
	pid_t pid;
	for (i = 0; i < n; i++)
	{
		pid = fork();
		if (pid == 0) return;
	}
}



void printstatus() {
	binary_sem_wait(semZ_id);
	binary_sem_wait(semY_id);
	int i;	
		printf("Y=[");
	for (i = 1; i < BY[0]+1; ++i)
		printf("%d ,", BY[i]);
	printf("]\n");

	printf("Z=[");
	for (i = 1; i < BZ[0]+1; ++i)
		printf("%d ,", BZ[i]);
	printf("]\n");

	printf("YZ=[");
	for (i = 1; i < YZ[0]+1; ++i)
		printf("%d ,", YZ[i]);
	printf("]\n----\n");

	binary_sem_post(semZ_id);
	binary_sem_post(semY_id);
	sleep(1);
}

int main(int argc, char* argv[]) {

	int i;

	if (argc == 7) {
		M = atoi(argv[1]);
		N = atoi(argv[2]);
		P = atoi(argv[3]);
		R = atoi(argv[4]);
		S = atoi(argv[5]);
		T = atoi(argv[6]);
		seg_size = (5 + 1) * sizeof(int);
	}
	else { printf("give 7 ints"); return 1; }

	Y_id = shmget(0, seg_size, 0666 | IPC_CREAT);
	Z_id = shmget(0, seg_size, 0666 | IPC_CREAT);
	YZ_id = shmget(0, 11*(sizeof(int)), 0666 | IPC_CREAT);
	if (Y_id<0 || Z_id<0) {
		printf("stats: shmget error");
		exit(1);
	}
	BY = shmat(Y_id, 0, 0);
	BZ = shmat(Z_id, 0, 0);
	YZ = shmat(YZ_id, 0, 0);

	if ((semY_id = binary_sem_allocate(semY_key, 0666 | IPC_CREAT)) < 0) exit(1);
	if ((semZ_id = binary_sem_allocate(semZ_key, 0666 | IPC_CREAT)) < 0) exit(1);

	if ((try_bufY = sem_allocate(try_bufY_key, 0666 | IPC_CREAT)) < 0) exit(1);
	if ((try_bufZ = sem_allocate(try_bufZ_key, 0666 | IPC_CREAT)) < 0) exit(1);
	if ((try_bufYZ = sem_allocate(try_bufYZ_key, 0666 | IPC_CREAT)) < 0) exit(1);
	if ((Yfree_id = sem_allocate(Yfree_key, 0666 | IPC_CREAT)) < 0) exit(1);
	if ((Zfree_id = sem_allocate(Zfree_key, 0666 | IPC_CREAT)) < 0) exit(1);

	if ((Yfull_id = sem_allocate(Yfull_key, 0666 | IPC_CREAT)) < 0) exit(1);
	if ((Zfull_id = sem_allocate(Zfull_key, 0666 | IPC_CREAT)) < 0) exit(1);

	binary_sem_init(semY_id);
	binary_sem_init(semZ_id);

	sem_init(try_bufY, 10);
	sem_init(try_bufZ, 10);
	sem_init(try_bufYZ, 10);

	sem_init(Yfree_id, 5);
	sem_init(Zfree_id, 5);

	sem_init(Yfull_id, 0);
	sem_init(Zfull_id, 0);


	/*******************************************************************************
	*                                       work
	*******************************************************************************/
	fpid = getpid();

	if ((pid = fork()) == 0)
	{
		makeforks(M - 1);
		printf("prodY pid:%d\n", getpid());
		prodY();
	}
	else
	if ((pid = fork()) == 0)
	{
		makeforks(N - 1);
		printf("prodZ pid:%d\n", getpid());
		
		prodZ();
	}
	else
	if ((pid = fork()) == 0)
	{
		i = 1;
		makeforks(P - 1);
		printf("cons pid:%d\n", getpid());
		cons();
	}




	if (getpid() == fpid)
	{
		do
		{
			printstatus();
		} while (YZ[0] != 10);
		sleep(2);
		printstatus();
		printf("\nUSUWANKO\n");
		shmdt(BY);
		shmdt(BZ);
		shmctl(Y_id, IPC_RMID, 0);
		shmctl(Z_id, IPC_RMID, 0);

		binary_sem_deallocate(semY_id);
		binary_sem_deallocate(semZ_id);
		sem_deallocate(Yfree_id);
		sem_deallocate(Zfull_id);
		sem_deallocate(Zfree_id);
		sem_deallocate(Yfull_id);
		sem_deallocate(try_bufY);
		sem_deallocate(try_bufZ);
		sem_deallocate(try_bufYZ);
	}
	return 0;
}



int prodY()
{
	int item;
	srand(getpid());
	while (sem_check(try_bufY))
	{
		sleep(rand() % R + 1);
		item = rand() % 10;
		sem_down(Yfree_id);
				binary_sem_wait(semY_id);
					printf("Y <- %d pid:%d \n", item, getpid());
					queueInsert(BY, item);
				binary_sem_post(semY_id);
		sem_up(Yfull_id); 
	}

	
}

int prodZ()
{
	int item;
	srand(getpid());
	while (sem_check(try_bufZ))
	{
		sleep(rand() % S + 1);
		item = rand() % 10;
		sem_down(Zfree_id);
				binary_sem_wait(semZ_id);
					printf("Z <- %d pid:%d \n", item, getpid());
					queueInsert(BZ, item);
				binary_sem_post(semZ_id);
		sem_up(Zfull_id);

	}

}

int cons()
{
	int item;
	srand(getpid());
	while (sem_check(try_bufYZ))
	{
		sleep(rand() % T + 1);
		sem_down(Yfull_id);
		sem_down(Zfull_id);
			binary_sem_wait(semZ_id);
			binary_sem_wait(semY_id);
				item = (queuePop(BY) * 10 + queuePop(BZ));
				queueInsert(YZ, item);
				printf("YZ <- %d pid:%d \n", item, getpid());
			binary_sem_post(semY_id);
			binary_sem_post(semZ_id);	
		sem_up(Zfree_id);
		sem_up(Yfree_id);	
	}
} 