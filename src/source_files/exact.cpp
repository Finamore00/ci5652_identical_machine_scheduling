#include "../header_files/vicinities.h"
#include <algorithm>
#include <vector>
#include <climits>

using namespace std;

/**
 * Function that takes in a schedule for the machines and calculates the total tardiness.
 */
long long total_tardiness(vector<vector<Job>> schedule) {
    long long tardiness = 0;
    for (vector<Job> machine : schedule) {
        long long completion_time = 0;
        for (Job job : machine) {
            completion_time += job.ptime;
            long long tard = max(0LL, completion_time - job.dDate);
            tardiness += tard;
        }
    }
    return tardiness;
}

/**
 * Function that takes in a list of jobs and a list of schedules, and a mask 
 * representing the jobs that have been assigned. It uses backtracking to assign 
 * each job to a schedule and calculates the total tardiness of all schedules.
 * The function returns a pair containing the minimum total tardiness and the corresponding schedule.
 */
pair<long long, vector<vector<Job>>> backtrack(vector<Job> jobs, vector<vector<Job>> schedules, long long mask) {
    if (mask == 1 << jobs.size()) {
        long long tardiness = 0;
        for (vector<Job> machine : schedules) {
            tardiness += machine_tardiness(machine);
        }
        return {tardiness, schedules};
    }
    
    pair<long long, vector<vector<Job>>> sol = {LLONG_MAX, {}};
    for (int i = 0; i < jobs.size(); i++) {
        if (!(mask & (1 << i))) {
            for (int j = 0; j < schedules.size(); j++) {
                schedules[j].push_back(jobs[i]);
                pair<long long, vector<vector<Job>>> curr = backtrack(jobs, schedules, mask | (1 << i));
                if (curr.first < sol.first) {
                    sol = curr;
                }
                schedules[j].pop_back();
            }
        }
    }
    return sol;
}

/**
 * Function that takes in a list of jobs and the number of machines, and uses 
 * backtracking to find the exact solution to the problem of minimizing the total 
 * tardiness of all machines. The function initializes a list of schedules, one for each machine, 
 * and then calls the backtrack function to assign each job to a schedule.
 * The function returns a pair containing the minimum total tardiness and the corresponding schedule.
 */
pair<long long, vector<vector<Job>>> exact_solution(vector<Job> jobs, int machines) {
    vector<vector<Job>> schedules(machines);
    return backtrack(jobs, schedules, 0);
}