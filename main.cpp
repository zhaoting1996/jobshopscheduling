#include "Commons.hpp"
#include "ProblemInstance.hpp"
#include "Constructives/ConstructiveGreedy.hpp"
#include "Constructives/Grasp.hpp"
#include "DeltaMoves/DeltaMoveExchange.hpp"
#include "DeltaMoves/DeltaMoveOpt1.hpp"
#include "Evaluator.hpp"

#include <stdlib.h>     /* atof */
#include <unistd.h>		/* getopt */

int main(int argc, char** argv){

	int opt;
	string instance_name = "abz5_f13.txt";
//	string instance_name = "instance.txt";
	double alpha_grasp = 0.5;
	int repeat = 1;
	string modo = "grasp2";

	while ((opt = getopt(argc, argv, "i:a:m:r:")) != -1) {
		switch(opt){
			case 'i':{
				// instance name
				instance_name = optarg;
			}break;

			case 'a':{
				// alpha value
				alpha_grasp = atof(optarg);
			}break;

			case 'm':{
				// constructive mode
				modo = optarg;
			}break;

			case 'r':{
				// repeat times
				repeat = atoi(optarg);
			}

		}
	}

	srand(time(NULL));

	Solution solution;
	// instance load
	ProblemInstance instance;
	instance.set_name_file(instance_name);
	if(!instance.load_instance()){
		exit(EXIT_FAILURE);
	}
	//instance.print();

	int nJobs = instance.get_num_jobs();
	int nTasks = instance.get_num_tasks();
	int nMachines = instance.get_num_machines();

	if(modo.compare("grasp1") == 0){
		Grasp grasp(instance, alpha_grasp);
		grasp.set_repeat(repeat);
		solution = grasp.apply_grasp1();

		grasp.print_graph();
		grasp.print();
	} else if(modo.compare("sprt") == 0){		
		Grasp grasp(instance, alpha_grasp);
		grasp.set_repeat(repeat);
		solution = grasp.grasp_SPRT();

		grasp.print_graph();
		//grasp.print();
	} else if(modo.compare("grasp2") == 0){
		Grasp grasp(instance, alpha_grasp);
		grasp.set_repeat(repeat);
		solution = grasp.apply_grasp2();

		grasp.print_graph();
		grasp.print();
	}
/*
	Evaluator evaluator(instance);
	DeltaMoveExchange exchange_move(instance);
	for(int i=0 ; i<nTasks ; i++){
		exchange_move.set_solution(solution);
		exchange_move.set_positions(0,1,i);
		solution = exchange_move.apply_exchange_move();
		print_solution(solution);
		evaluator.evaluate_solution(solution);
		evaluator.print();
	}
*/	

	return 0;
}