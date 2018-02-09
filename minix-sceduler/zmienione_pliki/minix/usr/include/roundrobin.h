#include <lib.h>
/*
	Zwraca liczbę kolejki procesu o zadanym PID lub EINVAL w przypadku braku procesu o zadanym PID
*/
int getqueue(pid_t pid)
{
	message m;
	m.m1_i1 = pid;
	return _syscall(MM, GETQUEUE, &m);
}
/*
	Ustawia liczbę kolejki procesu o zadanym PID. Funkcja zwraca:
	
	1,2 w przypadku powodzenia
	EINVAL w przypadku nie znalezienia procesu lub wartosci spoza dozwolnego zakresu
*/
int setqueue(pid_t pid, unsigned short new_lot)
{
	message m;
	m.m1_i1 = pid;
	m.m2_l2 = new_lot;
	return _syscall(MM, SETQUEUE, &m);
}
