#ifndef GRASP_HPP
#define GRASP_HPP

#include <Commons.hpp>
#include <ProblemInstance.hpp>
#include <LocalSearches/LocalSearches.hpp>
#include <Constructives/Constructives.hpp>
#include <Metaheuristic.hpp>

#include <functional>

using namespace twtjssp;

class Grasp : public Metaheuristic {

public:
    Grasp() = default;
    explicit Grasp(double _alpha = 0);

	Solution apply() override;	/*!< applies the grasp method */
	void print() override;
    void print_graphic() override;
    void print_method_informations() override;

    virtual Solution refinement(Solution&);

protected:

	Metaheuristic* local_search;
	Constructive* constructive;

    double alpha;   			/*!< defines how greedy will be your grasp  */

};

#endif