#include "monitor.h"

class HelpMonitor : public Monitor
{
  int tank;
public:
  HelpMonitor(): tank(0){};
  void inc()
  {
    enter();
    tank++;
    leave();
  }
  void dec()
  {
    enter();
    tank--;
    leave();
  }
  int check()
  {
  	enter();
  	int ret = tank;
  	if(tank) --tank;
  	leave();
  	return ret;
  }
  int get()
  {
    enter();
    int ret = tank;
    leave();
    return ret;
  }
  void set()
  {
    enter();
    tank = 1;
    leave();
  }
  void reset()
  {
    enter();
    tank = 0;
    leave();
  }
  
};