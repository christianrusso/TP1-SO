#ifndef __SCHED_LOT__
#define __SCHED_LOT__

#include <map>
#include <vector>
#include <stdlib.h>
#include "basesched.h"

#define BASE_TICKETS 1000

using std::map;
using std::vector;

class SchedLottery : public SchedBase {
  public:
    SchedLottery(std::vector<int> argn);
        ~SchedLottery();
    virtual void load(int pid);
    virtual void load(int pid, int deadline);
    virtual void unblock(int pid);
    virtual int tick(int cpu, const enum Motivo m);

  private:
  	int cpuQuantum;
  	vector<int> quantumActual;
  	map<int, int> procesos; 	  // <pid,tickets>
  	map<int, int> bloqueados; 	// <pid,#ciclos usados> 
  	int ticketsTotales;
  	int run_idle_task(int cpu);
  	int loteria(int pid, int cpu);
};

#endif
