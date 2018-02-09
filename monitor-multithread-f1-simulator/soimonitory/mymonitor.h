#include "helpmonitor.h"
#include <iostream>
#include <deque>
HelpMonitor zatankowano;

using namespace std;
//N bolidow, K stanowisk, P-zmiana priorytetu, L-liczba tankowan
int N, K, P, L;
class MyMonitor: public Monitor
{
public:
  MyMonitor(){wolnyPas.set();};

  void zwolnijPas()
  {
  		if(aleja.get() < P ) 
  		{  //priorytet dla wjeżdzających
	        if(chceWjazd.get() > 0 && aleja.get() < K)
	        { //jesli istnieje ktoś kto chce wyjechać to puszczamy,
	            chceWjazd.dec();
	            signal(pasWjazdowy);
	        } 
	        else if(chceWyjazd.get() > 0) 
	        { //jeśli nie to jesli isntieje ktoś kto chce wjechać to puszczamy
	            chceWyjazd.dec();
	            signal(pasWyjazdowy);
	        } 
	        else 
	        { // jeśli nie to pas wolny (pusty), moze zostac zajety przez dowolny pierwszy bolid ktory będzie chciał wjechać
	            wolnyPas.set();
	        }
    	} 
	    else 
	    { //priorytet dla wyjeżdzających, analogicznie
	        if(chceWyjazd.get() > 0)
	        { 
	            chceWyjazd.dec();
	            signal(pasWyjazdowy);
	        }
	        else if (chceWjazd.get() > 0 && aleja.get() < K)
	        {
	            chceWjazd.dec();
	            signal(pasWjazdowy);
	        } 
	        else
            {
           		wolnyPas.set();
    	    }
   		 } 
    
 }


  
  void zjazd(int proc_id)
  { 
   

	cout << proc_id << " chce zatankowac\n";
   	if(aleja.get() < K && wolnyPas.check()) 
	{
		;
	}
	else
	{
		chceWjazd.inc();
		wait(pasWjazdowy);
	}

    enter();


 
    	sleep(1); //jazda po pasie
    	aleja.inc();
    	zwolnijPas();
        cout << proc_id << "<- tankuje\n";

    leave();
  }
  
  void wyjazd(int proc_id, int tank)
  {

  	cout << proc_id << " chce wyjechac\n";
	if(wolnyPas.check())
	{
		; //check zaklepuje pas i przechodzi dalej
	}
	else
	{
		chceWyjazd.inc();
		wait(pasWyjazdowy);
	}



	enter();    
	   

    	sleep(1); //jazda po pasie
    	aleja.dec();
    	zwolnijPas();
     	cout << proc_id << " wyjechal ->\n";
     leave();
  }
  
private:
  Condition pasWjazdowy, pasWyjazdowy;
  HelpMonitor wolnyPas;
  HelpMonitor chceWjazd;
  HelpMonitor chceWyjazd;
  HelpMonitor aleja;
};