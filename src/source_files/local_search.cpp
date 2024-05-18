#include <iostream>
#include <vector>
#include "../header_files/Job.h"
#include "../header_files/heuristic.h"
#include "../header_files/vicinities.h"
#include "../header_files/exact.h"

using namespace std;

void prints(vector<vector<Job*>> &schedule, int machine_count) {
    for (int i = 0; i < machine_count; i++) {
        cout << "\nMachine " << i + 1 << ": ";
        for (Job* job : schedule[i]) {
            cout << job->id << " ";
        }
        cout << endl;
    }
}

vector<vector<Job*>> generate_random_solution(vector<Job*> jobs, int machine_count) {
    srand((unsigned)time(0));
    vector<vector<Job*>> schedule(machine_count);

    for (int i = 0; i < machine_count; i++) {
        schedule[i].resize(0);
    }

    for (int i = 0; i < jobs.size(); i++) {
        schedule[rand() % machine_count].push_back(jobs[i]);
    }

    return schedule;
}

/**
 * Implementation of local search algorithm for optimizing minimum total tardiness
 * in the parallel identical machine scheduling problem.
*/
vector<vector<Job*>> local_search(vector<Job*> jobs, int machine_count, unsigned int max_iter) {
    //Generate initial solution using heuristic algorithm
    vector<vector<Job*>> curr_solution = mddScheduling(jobs, machine_count);
    unsigned int iter_counter = 0;

    while (iter_counter++ < max_iter) {
        long long curr_total_tardiness = total_tardiness(curr_solution);
        
        // Find tardiest machine in the solution
        int tardiest_machine = -1;
        long long max_tardiness = LLONG_MIN;
        for (int i = 0; i < machine_count; i++) {
            if (machine_tardiness(curr_solution[i]) > max_tardiness) {
                tardiest_machine = i;
            }
        }

        vector<vector<Job*>> neighbor = generate_neighbor(curr_solution, tardiest_machine);
        prints(neighbor, machine_count);
        if (total_tardiness(neighbor) < curr_total_tardiness) {
            curr_solution = neighbor;
            iter_counter = 0;
        }

    }

    return curr_solution;
}