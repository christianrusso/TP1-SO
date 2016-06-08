#include "sched_rr2.h"

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	cantCores = argn[0];

	cpuQuantum = vector<int>(cantCores, 0);
	quantumActual = vector<int>(cantCores, 0);
	int i;
	for (i = 0; i < cantCores; i++)
	{
		quantumActual[i] = argn[i + 2];
		cpuQuantum[i] = argn[i + 2];
	}

	ready = vector< queue<int> >(cantCores, queue<int>());
	bloqueados = vector<int>(cantCores, 0);
	nucleo = map<int,int>();
}

SchedRR2::~SchedRR2() {

}


void SchedRR2::load(int pid) {
	int core = -1;
	int min = -1;
	int i;
	for (i = 0; i < cantCores; i++)
	{
		int activos = 0;

		if (current_pid(i) != -1)
			activos += 1;

		activos += bloqueados[i];
		activos += ready[i].size();

		if (activos < min || min == -1)
		{
			min = activos;
			core = i;
		}

	}

	ready[core].push(pid);
	nucleo[pid] = core;
}

void SchedRR2::unblock(int pid) {
	int core = nucleo[pid];
	bloqueados[core]--;
	ready[core].push(pid);
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
	if (m == EXIT || m == BLOCK)
	{
		if (m == BLOCK)
			bloqueados[cpu]++;

		if (ready[cpu].empty())
			return run_idle_task(cpu);
		else
		{
			quantumActual[cpu] = cpuQuantum[cpu];
			int prox = ready[cpu].front();
			ready[cpu].pop();
			return prox;
		}
	}
	else
	{ 	// Vino Tick
		if (current_pid(cpu) == IDLE_TASK)
		{
			if (ready[cpu].empty())
				return run_idle_task(cpu);
			else
				return proximo_proceso(cpu);
		}
		else
		{
			quantumActual[cpu]--;

			if (quantumActual[cpu] == 0)
			{
				if (ready[cpu].empty()){
					quantumActual[cpu] = cpuQuantum[cpu];
					return current_pid(cpu);
				}
				else
					return proximo_proceso(cpu);
			}
			else
			{
				return current_pid(cpu);
			}
		}
	}
}

int SchedRR2::proximo_proceso(int cpu)
{
	quantumActual[cpu] = cpuQuantum[cpu];
	int prox = ready[cpu].front();
	ready[cpu].pop();
	if (current_pid(cpu) != -1)
		ready[cpu].push(current_pid(cpu));
	return prox;
}

int SchedRR2::run_idle_task(int cpu)
{
	quantumActual[cpu] = cpuQuantum[cpu];
	return IDLE_TASK;
}
