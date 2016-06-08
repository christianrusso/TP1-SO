#include "tasks.h"
#include <iostream>
#include <vector>

using namespace std;

void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
	uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i]);
		else uso_IO(pid, params[i]);
	}
}

void TaskConsola(int pid, vector<int> params){
	for (int i = 0; i < (int)params[0]; i++) {
		uso_IO(pid,params[1] + rand() % (params[2] - params[1] + 1));
	}
}

void TaskBatch(int pid, vector<int> params)
{
	int ciclosTotales = params[0];
	int entradasBloqueantes = params[1];
	vector<int> ocupado(ciclosTotales,0);
	
	for (int i = 0; i < entradasBloqueantes; i++) {
		int momento = rand() % (ciclosTotales);

		while( ocupado[momento] )
			momento = rand() % (ciclosTotales);

		ocupado[momento] = 1;
	}

	for (int i = 0; i < ciclosTotales; i++) {
		if(ocupado[i])
			uso_IO(pid,1);
		else
			uso_CPU(pid,1);
	}		
			
}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConsola, 3);
	register_task(TaskBatch, 2);
}
