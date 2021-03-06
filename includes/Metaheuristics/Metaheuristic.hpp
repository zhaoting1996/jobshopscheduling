#ifndef METAHEURISTIC_HPP
#define METAHEURISTIC_HPP

#include <Commons.hpp>
#include <Solution.hpp>
#include <Evaluator.hpp>
#include <RandGen.hpp>

class Metaheuristic {

public:

    explicit Metaheuristic(Solution sol){
        solution = sol;
        graph = sol.getGraph();
        timeTotal = 0;
        atrasoInicial = 0;
        melhorAtraso = 0;
        iterAtual = 0;
        iterTotal = 0;
        sizeCriticalBlocks = 0;
        repeat=1;
    }

    Metaheuristic() = default;
    virtual ~Metaheuristic() = default;

    virtual Solution apply() = 0;

    void invert(Edge edge){
        graph.invert(edge.first, edge.second);
    }

    virtual void print(){
        float perc = percent_between(atrasoInicial, melhorAtraso);
        cout << endl;
        cout << "ATRASO INICIAL: " << atrasoInicial << endl;
        cout << "ATRASO FINAL: " << melhorAtraso << " (" << perc << "%)" << endl;
        bestSolution.print_solution();
    }

    virtual void print_graphic(){
        cout << atrasoInicial << "\t" << melhorAtraso << "\t" << timeTotal << endl;
    }

    void print_progress(){
        float progress = float(iterAtual)/float(iterTotal);
        int barWidth = 100;
        std::cout << "[";
        int pos = barWidth * progress;

        for (int i = 0; i < barWidth; i++) {
            if (i <= pos) std::cout << "\033[1;32m#\033[0m";
//            else if (i == pos) std::cout << "\033[1;31m>\033[0m";
            else std::cout << ".";
        }

        std::cout << "] " << int(progress * 100.0) << " %\r";
        std::cout.flush();

//        std::cout << std::endl;
    }

    virtual void print_method_informations(){
        cout << "==========================================================================================" << endl;
        cout << " -> METHOD <- " << endl;
    }

    void set_solution(Solution s){
        solution = s;
        set_graph(s.getGraph());
    }

    void set_graph(Graph g){
        graph = g;
    }

protected:
    Graph graph;
    Solution solution;
    Solution bestSolution;
    Evaluator evaluator;
    ProblemInstance instance;

    int atrasoInicial;
    int melhorAtraso;

    int iterAtual;
    int iterTotal;

    int repeat;
    int sizeCriticalBlocks;

    float timeTotal;
};

#endif