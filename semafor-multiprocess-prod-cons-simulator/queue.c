#include <time.h>
#include <math.h>
#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>

/*this function inserts a value to the end of the queue
 * permissions are checked earlier, by semaphores */
void queueInsert(int *queue, int val) {
	queue[queue[0] + 1] = val;
	++queue[0];
}

/*this function returns the first element of queue
 * moving the rest of them forward
 * permissions are checked earlier by semaphores */
int queuePop(int *queue) {
	int i;
	int ret = queue[1]; 
	for(i=1; i<queue[0]; ++i) {
		queue[i] = queue[i+1];
	}
	--queue[0];
	return ret;
}
/*returns the number of elements in queue
 * permissions are checked by semaphores */
int getQueueSize(int *queue) {
	return queue[0];
}