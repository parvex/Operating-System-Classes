#include <roundrobin.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int ArgC, char ** ArgV)
{
	
	int MyPriority = 0;
	int MyPid;
	int ChangeResult;
	unsigned int I, J;
	if(ArgC != 2)
	{
		printf("Uzycie: %s <priorytet tej aplikacji>\n", ArgV[0]);
		return 1;
	}
	
	MyPriority = atoi(ArgV[1]);
	MyPid = getpid();
	
	if(MyPriority < 1 || MyPriority > 3)
	{
		printf("");
	}
	
	printf("== Zmieniam (%d) kolejke na: %d\n", MyPid, MyPriority);
	ChangeResult = setqueue(MyPid, MyPriority);
	printf("== Wynik zmiany kolejki: %d\n", ChangeResult);
	MyPriority = getqueue(MyPid);
	printf("== Status po zmianie: %d\n", MyPriority);
	
	for(I = 0; I < 16; I++)
	{
		for(J = 0; J < 65535 * 256; J++) {}
	}
	
	printf("== Koncze: %d\n", MyPriority);

	return 0;
}
