#include "mymonitor.h"

#include <iostream>
#include <queue>
#include <pthread.h>
#include <time.h>
using namespace std;

MyMonitor mon;





void* bolid(void* args)
{
	srand(time(NULL));
	int n = *((int*)args);
	int tank = 0;
	cout << "bolid nr: " << n << endl;
	while(tank < L)
	{
		sleep(rand()% 5 + 1); //jazda
		mon.zjazd(n); //czekanie w kolejce i wjazd do aleji
		sleep(2);  //tankowanie
		mon.wyjazd(n, ++tank); //czekanie w kolejce i wyjazd z aleji
	}

	cout << n << " konczy jazde\n";
    
}


int main(int argc, char* argv[]) {


	if (argc == 5) {
		N = atoi(argv[1]);
		K = atoi(argv[2]);
		P = atoi(argv[3]);
		L = atoi(argv[4]);
	}
	else { printf("give 3 decreasing ints"); return 1; }

	int tabi[N];

	for(int i = 1; i <= N; ++i)
		tabi[i-1] = i;

	pthread_t* tab = new pthread_t[N];

 	for(int i = 0; i < N; ++i)
 		pthread_create(&tab[i], NULL, bolid, &tabi[i]);

 	for(int i = 0; i < N; ++i)
 		pthread_join(tab[i], NULL);

	delete tab;

  return 0;
}
