#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include <iostream>
#include "../header_files/utilities.h"
#include "../header_files/vicinities.h"
#include "../header_files/exact.h"
#include "../header_files/heuristic.h"

using namespace std;

/**
 * Implements the Random Neighborhood Algorithm (RNA) for scheduling jobs across multiple machines.
 *
 * @param schedule The initial schedule of jobs across machines.
 * @param rnamax The maximum number of iterations for the RNA algorithm.
 * @return A vector of vectors where each inner vector represents the schedule of jobs for a machine, optimized to minimize tardiness.
 * 
 * The function iteratively generates neighboring schedules and accepts them if they reduce the total tardiness.
 * The process is repeated for a specified number of iterations (`rnamax`).
 */
vector<vector<Job*>> rna(vector<vector<Job*>> schedule, int rnamax) {
    int i = 1;
    vector<vector<Job*>> best_schedule = schedule;
    vector<vector<Job*>> current_schedule = best_schedule;
    long long best_tardiness = total_tardiness(best_schedule);
    
    // Main loop for the RNA algorithm.
    while (i < rnamax) {
        // Generate a neighboring schedule.
        current_schedule = generate_neighbor(current_schedule, index_tardiest_machine(current_schedule));
        
        // If the neighboring schedule is better, accept it and reset the iteration counter.
        if (total_tardiness(current_schedule) < best_tardiness) {
            i = 0;
            best_schedule = current_schedule;
            best_tardiness = total_tardiness(current_schedule);
        }
        
        i++;
    }
    
    return best_schedule;
}


/**
 * Implements the Iterated Local Search (ILS) algorithm for scheduling jobs across multiple machines.
 *
 * @param jobs A vector of job pointers that need to be scheduled.
 * @param machines The number of machines available for scheduling.
 * @param max_iter The maximum number of iterations for the ILS algorithm.
 * @param p0 The initial perturbation strength.
 * @param pmax The maximum perturbation strength multiplier.
 * @param rnamax The maximum number of iterations for the RNA algorithm within ILS.
 * @param itermax The maximum number of iterations before increasing the perturbation strength.
 * @return A vector of vectors where each inner vector represents the schedule of jobs for a machine, optimized to minimize tardiness.
 * 
 * The function starts with an initial schedule using the `mddScheduling` function and then applies the RNA algorithm to improve it.
 * It iteratively applies perturbations and RNA to explore the search space, aiming to escape local optima and find better schedules.
 */
vector<vector<Job*>> ils(vector<Job*> jobs, int machines, unsigned int max_iter, unsigned int p0, unsigned int pmax, unsigned int rnamax, unsigned int itermax) {
    // Initialize the best and current schedules using the initial scheduling method.
    vector<vector<Job*>> best_schedule = mddScheduling(jobs, machines);
    vector<vector<Job*>> current_schedule = best_schedule;
    
    // Apply RNA to the initial schedule.
    best_schedule = rna(current_schedule, rnamax);
    current_schedule = best_schedule;
    
    long long best_tardiness = total_tardiness(best_schedule);
    int i = 0;
    int cnt = max_iter;
    int p = p0;
    pmax = p0 * pmax;

    // Main loop for the ILS algorithm.
    while (cnt--) {
        // Apply perturbations to the current schedule.
        for (int j = 0; j < p; j++) {
            current_schedule = generate_neighbor(current_schedule, index_tardiest_machine(current_schedule));
        }
        
        // Apply RNA to the perturbed schedule.
        current_schedule = rna(current_schedule, rnamax);
        
        // If the perturbed and improved schedule is better, accept it and reset the perturbation strength.
        if (total_tardiness(current_schedule) < best_tardiness) {
            best_schedule = current_schedule;
            best_tardiness = total_tardiness(current_schedule);
            p = p0;
        } else {
            // Otherwise, revert to the best known schedule and increase the iteration counter.
            current_schedule = best_schedule;
            i++;
        }

        // If the iteration counter reaches the limit, increase the perturbation strength.
        if (i >= itermax) {
            i = 0;
            p += p0;
            // Ensure the perturbation strength does not exceed the maximum.
            if (p > pmax) {
                p = p0;
            }
        } 
    }
    
    return best_schedule;
}
