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

vector<vector<Job*>> rna(vector<vector<Job*>> schedule, int rnamax) {
    int i = 1;
    vector<vector<Job*>> best_schedule = schedule;
    vector<vector<Job*>> current_schedule = best_schedule;
    long long best_tardiness = total_tardiness(best_schedule);
    while (i < rnamax) {
        current_schedule = generate_neighbor(current_schedule, index_tardiest_machine(current_schedule));
        if (total_tardiness(current_schedule) < best_tardiness) {
            int i = 0;
            best_schedule = current_schedule;
        }
        i++;
    }
    return best_schedule;
}

vector<vector<Job*>> ils(vector<Job*> jobs, int machines, unsigned int max_iter, unsigned int p0, unsigned int pmax, unsigned int rnamax,unsigned int itermax) {
    vector<vector<Job*>> best_schedule = mddScheduling(jobs, machines);
    vector<vector<Job*>> current_schedule, neighbor;
    current_schedule = best_schedule;
    best_schedule, current_schedule = rna(current_schedule, rnamax);
    long long best_tardiness, current_tardiness = total_tardiness(best_schedule);
    int i = 0;
    int cnt = max_iter;
    int p = p0;
    pmax = p0*pmax;

    while (cnt--) {
        for (int j = 0; j < p; j++) {
            current_schedule = generate_neighbor(current_schedule, index_tardiest_machine(current_schedule));
        }
        current_schedule = rna(current_schedule, rnamax);
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
            if(p > pmax) {
                p = p0;
            }
        } 
    }
    return best_schedule;
}