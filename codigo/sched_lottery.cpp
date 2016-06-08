#include "sched_lottery.h"

using namespace std;

SchedLottery::SchedLottery(vector<int> argn) {
  // Lottery recibe dos parametros: el quantum y la semilla de la secuencia pseudoaleatoria.
	int cores = argn[0];
	cpuQuantum = argn[1];
	int semilla = argn[2];
	srand(semilla);
	quantumActual = vector<int>(cores, cpuQuantum);
	procesos = map<int, int>();
	bloqueados = map<int, int>();
	ticketsTotales = 0;
}

SchedLottery::~SchedLottery() {
}

void SchedLottery::load(int pid) {
	load(pid, 0);
}

void SchedLottery::load(int pid,int deadline) {
	procesos[pid] = BASE_TICKETS;
	ticketsTotales += BASE_TICKETS;
}

void SchedLottery::unblock(int pid) {
	// Agrego el proceso pid a los procesos ready.
	procesos[pid] = (BASE_TICKETS * cpuQuantum / bloqueados[pid]);
	// Otorgo al proceso el compensation ticket con valor dependiendo de cuanto uso su quantum.
	ticketsTotales += procesos[pid]; // Compensation Ticket
}

int SchedLottery::tick(int cpu, const enum Motivo m)
{
	if (m == EXIT)
	{
		if (procesos.empty())
			return run_idle_task(cpu);
		else
			return loteria(-1, cpu);
	}
	else if (m == BLOCK)
	{
		quantumActual[cpu]--;
		bloqueados[current_pid(cpu)] = cpuQuantum - quantumActual[cpu];

		if (procesos.empty())
			return run_idle_task(cpu);
		else
			return loteria(-1, cpu);
	}
	else // Vino un TICK
	{
		if (current_pid(cpu) == IDLE_TASK)
		{
			if (procesos.empty())
				return run_idle_task(cpu);
			else
				return loteria(-1, cpu);
		}
		else
		{
			quantumActual[cpu]--;

			if (quantumActual[cpu] == 0)
			{
				if (procesos.empty())
					return current_pid(cpu);
				else
					return loteria(current_pid(cpu), cpu);
			}
			else
				return current_pid(cpu);
		}
	}
}

int SchedLottery::loteria(int pid, int cpu)
{
	// Reinicio el quantum para el proximo proceso
	quantumActual[cpu] = cpuQuantum;

	// Si el proceso anterior no termino ni se bloqueo, lo coloco nuevamente entre los procesos ready.
	if (pid != -1)
	{
		procesos[pid] = BASE_TICKETS;
		ticketsTotales += BASE_TICKETS;
	}

	// Determino el ticket ganador al azar.
	int ticketGanador = rand() % (ticketsTotales + 1);
	int procesoGanador = procesos.begin()->first;
	int sumatoria = 0;

	map<int, int>::iterator it;

	// Determino el proceso ganador de la loteria.
	for (it = procesos.begin(); it != procesos.end(); it++)
	{
		sumatoria += it->second;			// Sumo la cant de tickets del proceso

		if (sumatoria >= ticketGanador){
			procesoGanador = it->first;		// Obtengo el pid del ganador
			break;
		}
	}

	// Elimino los tickets del proceso ganador, lo quito de la lista de procesos ready y retorno su pid.
	ticketsTotales -= procesos[procesoGanador];
	procesos.erase(procesoGanador);
	return procesoGanador;
}

int SchedLottery::run_idle_task(int cpu)
{
	quantumActual[cpu] = cpuQuantum;
	return IDLE_TASK;
}
