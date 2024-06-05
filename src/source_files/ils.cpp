#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include <iostream>
#include "../header_files/utilities.h"
#include "../header_files/local_search.h"
#include "../header_files/vicinities.h"
#include "../header_files/exact.h"
#include "../header_files/heuristic.h"

using namespace std;

/**
 * Implements the Iterated Local Search (ILS) algorithm for scheduling jobs across multiple machines.
 *
 * @param jobs A vector of job pointers that need to be scheduled.
 * @param machines The number of machines available for scheduling.
 * @param max_iter The maximum number of iterations for the ILS algorithm.
 * @param p0 The initial perturbation strength.
 * @param pmax The maximum perturbation strength multiplier.
 * @param lsmax The maximum number of iterations for the local search algorithm within ILS.
 * @param itermax The maximum number of iterations before increasing the perturbation strength.
 * @return A vector of vectors where each inner vector represents the schedule of jobs for a machine, optimized to minimize tardiness.
 * 
 * The function starts with an initial schedule using the `mddScheduling` function and then applies the RNA algorithm to improve it.
 * It iteratively applies perturbations and LS to explore the search space, aiming to escape local optima and find better schedules.
 */
vector<vector<Job*>> ils(vector<Job*> jobs, int machines, unsigned int max_iter, unsigned int p0, unsigned int pmax, unsigned int lsmax, unsigned int itermax) {
    vector<vector<Job*>> best_schedule = mddScheduling(jobs, machines);
    vector<vector<Job*>> current_schedule = best_schedule;
    
    best_schedule = improve_solution_by_ls(current_schedule, machines, lsmax);
    current_schedule = best_schedule;
    
    long long best_tardiness = total_tardiness(best_schedule);
    int i = 0;
    int cnt = max_iter;
    int p = p0;
    pmax = p0 * pmax;

    while (cnt--) {
        for (int j = 0; j < p; j++) {
            current_schedule = generate_neighbor(current_schedule, index_tardiest_machine(current_schedule));
        }
        
        current_schedule = improve_solution_by_ls(current_schedule, machines, lsmax);
        
        if (total_tardiness(current_schedule) < best_tardiness) {
            best_schedule = current_schedule;
            best_tardiness = total_tardiness(current_schedule);
            p = p0;
        } else {
            current_schedule = best_schedule;
            i++;
        }

        if (i >= itermax) {
            i = 0;
            p += p0;
            if (p > pmax) {
                p = p0;
            }
        } 
    }
    
    return best_schedule;
}
