#include "GraspSPRT.hpp"

GraspSPRT::GraspSPRT() : Grasp(){

}

GraspSPRT::GraspSPRT(ProblemInstance p, double alpha) : Grasp(p, alpha) {

}

/*
 * GRASP CONSTRUCTIVE
 * Cria solucao considerando o custo das tarefas.
 * Aloca aquela que possuir o menor custo na solucao apos ser inserida
 * Em caso de empate de tarefas, opta-se por aquela que se inserida, deixa o menor tempo acumulado em sua maquina
 */
float GraspSPRT::define_priority(Schedule tarefa){
	// priority = s / RPT
	float s = instance.get_due_times()[tarefa.job];
	s -= solution.time_can_be_alocated(tarefa);

	float rpt = 0; // tempo de processamento restante 
	for(int i=tarefa.task ; i<instance[tarefa.job].size() ; i++){
		rpt += instance[tarefa.job][i].time_execution;
	}

	s -= rpt;
//	cout << "Job: " << tarefa.job << " " << tarefa.task << " " << s << " " << rpt << endl;
	return (float) (s / rpt); 
}
/*
int GraspSPRT::choose_schedule(){
	return 0;
}
*/
void GraspSPRT::print_graphic(){
	cout << alpha << " " << media_atraso << endl;
}