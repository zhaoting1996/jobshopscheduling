#include "Evaluator.hpp"

Evaluator::Evaluator(){
	this->total_atraso = 0;
}

Evaluator::Evaluator(ProblemInstance p){
	this->instance = p;
	this->total_atraso = 0;
}

Evaluator::Evaluator(Solution s, ProblemInstance p){
	this->solution = s;
	this->instance = p;
	this->total_atraso = 0;
}

int Evaluator::evaluate_solution(Solution solution){
	// data de conclusão é quando terminou
	// data de entrega é fornecido pela instancia

	this->solution = solution;
	int nMachines = this->instance.get_num_machines();
	int nJobs = this->instance.get_num_jobs();
	int nTasks = this->instance.get_num_tasks();
	ScheduleMatrix jobs = this->instance.get_vec_schedules();
	expectedTimes = this->instance.get_vec_conclusion_times();
	vector<int> priorities = this->instance.get_vec_priorities();
	realTimes.resize(nJobs);
	vector<int> tardiness(nJobs);

	total_atraso = 0;

	for(int i=0 ; i<nJobs ; i++){
		int lastMachine = jobs[i][nTasks-1].machine;
		for(int j=0 ; j<nTasks ; j++){
			if(solution[lastMachine][j].job == i){
				realTimes[i] = (solution[lastMachine][j].time_execution);
				tardiness[i] = realTimes[i] - expectedTimes[i];
				if(tardiness[i] < 0){
					tardiness[i] = 0;
				}
				total_atraso += (tardiness[i]);
				break;
			}
		}
	}

	testa_solucao(solution);

	return total_atraso;
}

int Evaluator::analisa_job(Schedule tarefa, Solution solution_temp){
	/*
	 * Retorna o instante em que, caso a tarefa fosse inserida, ela seria concluida
	 */

	ScheduleMatrix jobs = instance.get_vec_schedules();
	int machine = tarefa.machine;
	int tempo = 0;

	if(tarefa.task == 0){
		// se for a primeira tarefa, apenas e necessario analisar o tempo dela
		if(solution_temp[machine].size() == 0){
			// caso nao tenham tarefas alocadas na maquina
			tempo = tarefa.time_execution;
		} else {
			// caso ja tenham tarefas alocadas na maquina, retorna o tempo acumulado dessa maquina
			int size = solution_temp[machine].size();
			tempo = solution_temp[machine][size-1].time_execution + tarefa.time_execution;
		}

	} else {
		int lastMachine = jobs[tarefa.job][tarefa.task-1].machine; // maquina referente a ultima task executada
		int sizeAtualMachine = solution_temp[machine].size(); // quantidade de tarefas na maquina atual
		int timeLastTask; // tempo da ultima tarefa executada do job

		// procura pela ultima task executada do job e armazena o instante de conclusao
		for(int i=0 ; i<solution_temp[lastMachine].size() ; i++){
			if(solution_temp[lastMachine][i].job == tarefa.job){
				timeLastTask = solution_temp[lastMachine][i].time_execution;
				break;
			}
		}

		// armazena o tempo acumulado da maquina atual
		int timeAtualMachine = 0;
		if(sizeAtualMachine > 0){
			timeAtualMachine = solution_temp[machine][sizeAtualMachine-1].time_execution;
		}

		if(timeLastTask > timeAtualMachine){
			/* Caso o instante da maquina atual seja menor que o instante que a ultima tarefa finalizou
			 * É necessário uma janela na producao da maquina, pois a proxima tarefa do job nao pode comecar 
			 * antes da anterior terminar
			 */
			tempo = timeLastTask + tarefa.time_execution;
		} else {
			/* Caso o instante em que a ultima tarefa do job terminou seja menor que o instante que a maquina
			 * atual se encontra, apenas é necessario inserir a tarefa na sequencia, sem janelas na producao.
			 * Nesse caso, a tarefa anterior terminou antes do que o tempo atual da maquina
			 */
			tempo = timeAtualMachine + tarefa.time_execution;
		}
	}

	// subtrai do tempo de conclusao
	tempo -= instance.get_vec_conclusion_times()[tarefa.job];
	// caso seja menor do que 0, significa que nao há atraso no job ate o momento
	if(tempo < 0) tempo = 0;

	// multiplica pela prioridade de cada job fornecido pela instancia
	tempo *= instance.get_vec_priorities()[tarefa.job];

	return tempo;
}

int Evaluator::analisa_machine(Schedule tarefa, Solution solution_temp){
	/*
	 * Retorna o tempo acumulado da maquina caso a tarefa fosse alocada
	 */

	ScheduleMatrix jobs = instance.get_vec_schedules();
	int machine = tarefa.machine;
	int tempo = 0;

	if(tarefa.task == 0){
		// se for a primeira tarefa, apenas e necessario analisar o tempo dela
		if(solution_temp[machine].size() == 0){
			// caso nao tenham tarefas alocadas na maquina
			tempo = tarefa.time_execution;
		} else {
			// caso ja tenham tarefas alocadas na maquina, retorna o tempo acumulado dessa maquina
			int size = solution_temp[machine].size();
			tempo = solution_temp[machine][size-1].time_execution + tarefa.time_execution;
		}

	} else {
		int lastMachine = jobs[tarefa.job][tarefa.task-1].machine; // maquina referente a ultima task executada
		int sizeAtualMachine = solution_temp[machine].size(); // quantidade de tarefas na maquina atual
		int timeLastTask; // tempo da ultima tarefa executada do job

		// procura pelo tempo que terminou a ultima operacao do job candidato
		for(int i=0 ; i<solution_temp[lastMachine].size() ; i++){
			if(solution_temp[lastMachine][i].job == tarefa.job){
				timeLastTask = solution_temp[lastMachine][i].time_execution;
				break;
			}
		}

		int timeAtualMachine = 0;
		if(sizeAtualMachine > 0){
			timeAtualMachine = solution_temp[machine][sizeAtualMachine-1].time_execution; // tempo acumulado da maquina atual
		}

		if(timeLastTask > timeAtualMachine){
			// caso o tempo da maquina atual seja menor que o tempo que a ultima tarefa finalizou
			// cria uma janela na producao
			tempo = timeLastTask + tarefa.time_execution;
		} else {
			// caso o tempo que a ultima tarefa finalizou seja menor que o tempo acumulado da maquina atual
			// insere a tarefa na sequencia, sem janelas
			tempo = timeAtualMachine + tarefa.time_execution;
		}
	}

	return tempo;
}

bool Evaluator::testa_solucao(Solution solution){
	int nJobs = instance.get_num_jobs();
	int nTasks = instance.get_num_tasks();
	int nMachines = instance.get_num_machines();
	ScheduleMatrix jobs = instance.get_vec_schedules();

	for(int i=0 ; i<nJobs ; i++){
		int timeAtual = 0;
		int timeLast = 0;
		for(int j=0 ; j<nTasks ; j++){
			int machine = jobs[i][j].machine;
			timeAtual = 0;
			for(int k=0 ; k<nTasks ; k++){
				if(solution[machine][k].job == i){
					if(timeAtual < timeLast){
						cout << "ERRO NA SOLUCAO" << endl;
						timeLast = timeAtual;
						return false;
					}
				}
				timeAtual += solution[machine][k].time_execution;
			}
		}
	}

	return true;
}

void Evaluator::set_instance(ProblemInstance p){
	this->instance = p;
}

void Evaluator::set_solution(Solution s){
	this->solution = s;
}

void Evaluator::print(){
	for(int i=0 ; i<instance.get_num_jobs() ; i++){
		cout << "(" << realTimes[i] << "," << expectedTimes[i] << ") - ";
	}
	cout << endl << "TOTAL: " << total_atraso << endl;
}

void Evaluator::print_graph(){
	
}
