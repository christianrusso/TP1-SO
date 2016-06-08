#include "sched_rr.h"

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
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
}

SchedRR::~SchedRR() {
	
}


void SchedRR::load(int pid) {
	q.push(pid);
}

void SchedRR::unblock(int pid) {
	q.push(pid);		
}

int SchedRR::tick(int cpu, const enum Motivo m) {

	if (m == EXIT || m == BLOCK)
	{
		if (q.empty())
			return run_idle_task(cpu);
		else
		{
			quantumActual[cpu] = cpuQuantum[cpu];
			int prox = q.front();
			q.pop();
			return prox;
		}
	}
	else
	{ 	// Vino Tick
		if (current_pid(cpu) == IDLE_TASK)
		{
			if (q.empty())
				return run_idle_task(cpu);
			else
				return proximo_proceso(cpu);
		}
		else
		{
			quantumActual[cpu]--;

			if (quantumActual[cpu] == 0)
			{
				if (q.empty()){
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

int SchedRR::proximo_proceso(int cpu)
{
	quantumActual[cpu] = cpuQuantum[cpu];
	int prox = q.front();
	q.pop();
	if (current_pid(cpu) != -1)
		q.push(current_pid(cpu));
	return prox;
}

int SchedRR::run_idle_task(int cpu)
{
	quantumActual[cpu] = cpuQuantum[cpu];
	return IDLE_TASK;
}

