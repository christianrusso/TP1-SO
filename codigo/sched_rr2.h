#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <vector>
#include <queue>
#include <map>
#include "basesched.h"

class SchedRR2 : public SchedBase {
	public:
		SchedRR2(std::vector<int> argn);
        ~SchedRR2();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		int cantCores;
		std::vector<int> cpuQuantum;
		std::vector<int> quantumActual;
		std::vector< std::queue<int> > ready;
		std::vector<int> bloqueados;
		std::map<int,int> nucleo;
		int run_idle_task(int cpu);
		int proximo_proceso(int cpu);
};

#endif
