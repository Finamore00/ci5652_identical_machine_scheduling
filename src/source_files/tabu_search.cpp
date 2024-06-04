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


vector<vector<Job*>> tabu_search(vector<Job*> jobs, int machines, unsigned int max_iter, unsigned int max_grn_iter, unsigned int tabu_tenure) {
    vector<vector<Job*>> best_schedule = mddScheduling(jobs, machines);
    vector<vector<Job*>> current_schedule = best_schedule;
    unordered_set<Move, MoveHasher> tabu_list;
    long long best_tardiness = total_tardiness(best_schedule);
    int iter = 0;

    while (iter < max_iter) {
        int tardiest_machine = index_tardiest_machine(current_schedule);
        vector<vector<Job*>> best_candidate = current_schedule;
        long long best_candidate_tardiness = LLONG_MAX;
        Move best_move(-1, -1, -1);

        for (int i = 0; i < max_grn_iter; i++) {
            vector<vector<Job*>> candidate = generate_neighbor(current_schedule, tardiest_machine);
            long long candidate_tardiness = total_tardiness(candidate);
            if (candidate_tardiness < best_candidate_tardiness) {
                best_candidate = candidate;
                best_candidate_tardiness = candidate_tardiness;
                int idx1 = 0, idx2 = 0;
                while (candidate[tardiest_machine].size() > 1 && idx1 == idx2) {
                    idx1 = rand() % candidate[tardiest_machine].size();
                    idx2 = rand() % candidate[tardiest_machine].size();
                }
                
                best_move = Move(tardiest_machine, idx1, idx2);
            }
        }

        if (tabu_list.find(best_move) == tabu_list.end() || best_candidate_tardiness < best_tardiness) {
            current_schedule = best_candidate;
            tabu_list.insert(best_move);
            if (tabu_list.size() > tabu_tenure) {
                tabu_list.erase(tabu_list.begin());
            }
            if (best_candidate_tardiness < best_tardiness) {
                best_schedule = best_candidate;
                best_tardiness = best_candidate_tardiness;
            }
        }
        iter++;
    }
    return best_schedule;
}