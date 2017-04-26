#include "GraspPriority.hpp"

GraspPriority::GraspPriority(ProblemInstance p, double al) : Grasp(p, al){

}

float GraspPriority::define_priority(Schedule tarefa){
    /* Retorna o tempo de termino da operacao, dando uma prioridade caso
	 * sua operacao seguinte possa ser iniciada imediatamente apos a operacao atual, 
	 * evitando janelas na producao
	 */

	// tempo de conclusao da tarefa
	int conclusionTime = solution.time_can_be_alocated(tarefa) + tarefa.time_execution;

	if(tarefa.task < nOperations-1){
		int nextMachine = instance[tarefa.job][tarefa.task+1].machine;
		int timeNextMachine = 0;
		if(solution[nextMachine].size() > 0){
			timeNextMachine = solution[nextMachine].back().time_execution;
		}

		if(timeNextMachine <= conclusionTime){
			conclusionTime *= 0.5;
		}
	}

	// subtrai do tempo de conclusao 
	conclusionTime -= instance.get_due_times()[tarefa.job];
	
	// caso seja menor do que 0, significa que nao há atraso no job ate o momento
	if(conclusionTime < 0) conclusionTime = 0;

	// multiplica pela prioridade de cada job fornecido pela instancia
	//tempo *= instance.get_vec_priorities()[tarefa.job];

	return conclusionTime;
}

int GraspPriority::choose_schedule(){
    
    int menor = 0;
    if(restricts.size() > 1){
        /* 
         * Se considerar o menor tempo de processamento como criterio de desempate,
         * o atraso da solucao fica muito alto.
         * A melhor opcao é considerar a tarefa que implica no menor tempo acumulado em sua maquina.
         */

        vector<int> acumulatedTimes;
        for(int j=0 ; j<restricts.size() ; j++){
            // Retorna o tempo acumulado da maquina caso a tarefa fosse inserida
			int tempoAcumulado = solution.time_can_be_alocated(jobs_temp[restricts[j]][0]) + jobs_temp[restricts[j]][0].time_execution;
            acumulatedTimes.push_back(tempoAcumulado);
        }

        // Procura pelo menor
        for(int j=0 ; j<acumulatedTimes.size() ; j++){
            if(acumulatedTimes[j] < acumulatedTimes[menor]){
                menor = j;
            }
        }

    }
    
    return menor;	
}

void GraspPriority::print_graphic(){
    cout << alpha << " " << media_atraso << endl;
}