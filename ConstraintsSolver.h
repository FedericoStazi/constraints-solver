//
// Created by federico on 19/06/2020.
//

#ifndef CONSTRAINTS_SOLVER_CONSTRAINTSSOLVER_H
#define CONSTRAINTS_SOLVER_CONSTRAINTSSOLVER_H

#include <vector>
#include <map>
#include <queue>
#include <iostream>
#include <algorithm>
#include <cassert>
#include "Variable.h"

namespace csl {

    template<class var_type>
    class ConstraintsSolver {

    public:

        void print(bool print) {
            can_print = print;
        }

        void add_variable(var_type* var) {
            if (var->domain_size() == 1)
                ready_queue.push_back(var);
            else
                waiting_queue.insert(var);
        }

        void add_constraint(var_type* var1, var_type* var2) {
            if (!constraints.count(var1))
                constraints.insert({var1, {}});
            if (!constraints.count(var2))
                constraints.insert({var2, {}});
            constraints[var1].push_back(var2);
            constraints[var2].push_back(var1);
        }

        ///Returns true if a solution is found and it is unique
        bool solve_unique() {
            propagate();
            return solved();
        }

        ///Find any solution.
        ///Returns true if it can be solved, false if there is no solution.
        ///The search tree has max_depth as maximum depth (inf by default).
        ///If max_depth is not enough for determining a solution (or even if it is solvable), throws an exception.
        bool solve_any(int max_depth = std::numeric_limits<int>::max()) {
            //iterative deepening
            for(int depth = 1; depth <= max_depth; depth++) {
                if (can_print)
                    std::cout << std::endl << "Depth: " << depth << std::endl;
                solvable = true;
                if (next_choice(depth))
                    return solvable;
            }
            throw max_depth_too_small();
        }

    private:

        ///True if all the variables have unique values satisfying the constraints
        bool inline solved() {
            return solvable and waiting_queue.empty();
        }

        ///Propagates the constraints of all the unique valued variables not yet propagated
        void propagate() {

            std::vector<var_type*> log;

            while (!ready_queue.empty()) {

                auto var = ready_queue.back();
                ready_queue.pop_back();
                log.push_back(var);

                //propagate var constraints
                assert(var->has_unique_value());
                for (auto cons_var : constraints[var])
                    if (cons_var->remove(var)){
                        if (cons_var->domain_size() == 1) {
                            waiting_queue.erase(cons_var);
                            ready_queue.push_back(cons_var);
                        } else if (cons_var->domain_size() == 0) {
                            solvable = false;
                        }
                    }

                if (!solvable)
                    break;
            }

            ready_queue.clear();
            propagate_log.push_back(log);
        }

        ///Revert the effects of the last call to propagate()
        void revert() {

            std::vector<var_type*> log = propagate_log.back();
            propagate_log.pop_back();

            while (!log.empty()) {

                auto var = log.back();
                log.pop_back();

                //revert var constraints' effects
                assert(var->has_unique_value());
                for (auto cons_var : constraints[var]) {
                    cons_var->revert(var);
                    if (cons_var->domain_size() > 1 and !waiting_queue.count(cons_var))
                        waiting_queue.insert(cons_var);
                }

            }

        }

        ///Tests all the possible choices for a variable
        ///Returns false if result is not defined because depth was not enough, true otherwise
        bool explore(var_type* var, int depth) {

            if (!solvable)
                return true;

            if (depth == 0)
                return false;

            bool defined = true;
            waiting_queue.erase(var);
            choices_log.push_back(var);

            //Loop over possible choices
            while (var->next_choice()) {

                if (can_print)
                    print_choices();

                ready_queue.push_back(var);
                defined &= next_choice(depth - 1);
                if(solved())
                    return true;
                revert();
                solvable = true;
            }

            choices_log.pop_back();
            solvable = false;
            waiting_queue.insert(var);
            return defined;

        }

        ///Propagates the results of the last choice and, if no solution is found, makes the next choice
        ///Returns false if result is not defined because depth was not enough, true otherwise
        bool next_choice(int depth) {
            propagate();
            return waiting_queue.empty() or explore(*waiting_queue.begin(), depth);
        }


        ///Print current choices
        void print_choices() {
            for (var_type* v:choices_log) {
                std::cout << v->to_string() << "=";
                if (v->has_unique_value())
                    std::cout << v->unique_value().value() << " ";
                else
                    std::cout << "? ";
            }
            std::cout<<std::endl;
        }

    class max_depth_too_small : public std::exception {
            [[nodiscard]] const char* what() const noexcept override {
                return "max_depth is too small to solve the problem";
            }
        };


        ///False when an unsatisfiable variable is found
        bool solvable = true;


        ///Contains unique valued variables not yet propagated
        std::vector<var_type*> ready_queue;

        ///Contains non unique valued variables
        std::set<var_type*> waiting_queue;

        ///Contains all the variables' constraints
        std::map<var_type*, std::vector<var_type*>> constraints;


        ///Log of the propagate calls (used by revert())
        std::vector<std::vector<var_type*>> propagate_log;


        ///Log of the variable choices (used for printing info)
        std::vector<var_type*> choices_log;

        bool can_print = false;

    };
}


#endif //CONSTRAINTS_SOLVER_CONSTRAINTSSOLVER_H
