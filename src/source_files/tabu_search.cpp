#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include <iostream>
#include <unordered_set>
#include "../header_files/utilities.h"
#include "../header_files/vicinities.h"
#include "../header_files/exact.h"
#include "../header_files/heuristic.h"
#include "../header_files/Move.h"

using namespace std;

/**
 * Implements the Tabu Search algorithm for scheduling jobs across multiple machines.
 *
 * @param jobs A vector of job pointers that need to be scheduled.
 * @param machines The number of machines available for scheduling.
 * @param max_iter The maximum number of iterations for the Tabu Search algorithm.
 * @param max_grn_iter The maximum number of iterations for generating neighbors within each iteration.
 * @param tabu_tenure The number of iterations for which a move remains in the tabu list.
 * @return A vector of vectors where each inner vector represents the schedule of jobs for a machine, optimized to minimize tardiness.
 * 
 * The function begins by creating an initial schedule using the `mddScheduling` function and calculates its total tardiness.
 * If the initial tardiness is zero, the function returns the schedule immediately.
 * Otherwise, it iteratively improves the schedule by generating neighboring schedules and applying the Tabu Search strategy
 * to avoid cycles and local optima. The best found schedule is returned after the maximum number of iterations.
 */
vector<vector<Job*>> tabu_search(vector<Job*> jobs, int machines, unsigned int max_iter, unsigned int max_grn_iter, unsigned int tabu_tenure) {
    // Initialize the best and current schedules using the initial scheduling method.
    vector<vector<Job*>> best_schedule = mddScheduling(jobs, machines);
    vector<vector<Job*>> current_schedule = best_schedule;
    
    // Tabu list to keep track of forbidden moves.
    unordered_set<Move, MoveHasher> tabu_list;
    
    // Calculate the total tardiness of the initial schedule.
    long long best_tardiness = total_tardiness(best_schedule);
    
    // If initial tardiness is zero, return the schedule immediately.
    if(best_tardiness == 0) {
        return best_schedule;
    }
    
    int iter = 0;

    // Main loop for the Tabu Search algorithm.
    while (iter < max_iter) {
        // Find the index of the machine with the highest tardiness.
        int tardiest_machine = index_tardiest_machine(current_schedule);
        
        // Variables to store the best candidate schedule and its tardiness.
        vector<vector<Job*>> best_candidate = current_schedule;
        long long best_candidate_tardiness = LLONG_MAX;
        Move best_move(-1, -1, -1);

        // Generate neighbors and evaluate them.
        for (int i = 0; i < max_grn_iter; i++) {
            vector<vector<Job*>> candidate = generate_neighbor(current_schedule, tardiest_machine);
            long long candidate_tardiness = total_tardiness(candidate);
            
            // Update the best candidate if the current one is better.
            if (candidate_tardiness < best_candidate_tardiness) {
                best_candidate = candidate;
                best_candidate_tardiness = candidate_tardiness;
                
                // Select two random positions within the tardiest machine.
                int idx1 = 0, idx2 = 0;
                while (candidate[tardiest_machine].size() > 1 && idx1 == idx2) {
                    idx1 = rand() % candidate[tardiest_machine].size();
                    idx2 = rand() % candidate[tardiest_machine].size();
                }
                
                best_move = Move(tardiest_machine, idx1, idx2);
            }
        }

        // Check if the best move is not in the tabu list or if it improves the best known tardiness.
        if (tabu_list.find(best_move) == tabu_list.end() || best_candidate_tardiness < best_tardiness) {
            current_schedule = best_candidate;
            
            // Add the move to the tabu list.
            tabu_list.insert(best_move);
            
            // Maintain the tabu list size.
            if (tabu_list.size() > tabu_tenure) {
                tabu_list.erase(tabu_list.begin());
            }
            
            // Update the best schedule if the current candidate is better.
            if (best_candidate_tardiness < best_tardiness) {
                best_schedule = best_candidate;
                best_tardiness = best_candidate_tardiness;
            }
        }
        iter++;
    }
    return best_schedule;
}