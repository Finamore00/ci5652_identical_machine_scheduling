#include <iostream>
#include <vector>
#include "../header_files/Job.h"
#include "../header_files/local_search.h"
#include "../header_files/exact.h"
#include "../header_files/heuristic.h"
#include <chrono>
#include <utility>

using namespace std;
using namespace std::chrono;

/**
 * Generates a Restricted Candidate List (RCL) of jobs based on certain criteria.
 *
 * @param jobs The list of jobs to consider.
 * @param numMachines The number of machines available.
 * @param completion_times The completion times of the machines.
 * @param selected_machine The reference to the selected machine.
 * @param selected_job The reference to the selected job.
 * @param alpha The alpha value used to calculate the condition for RCL.
 * @return A vector of pairs, where each pair contains a job and the corresponding machine.
 */
vector<pair<Job *, int>> RCL(vector<Job *> jobs, int numMachines, vector<long long> completion_times, int &selected_machine, Job* &selected_job, float alpha) {
    long long max_delta = -1;
    long long min_delta = LLONG_MAX;

    vector<pair<Job *, long long>> cantidate_jobs(numMachines);
    for (int machine = 0; machine < numMachines; machine++) {
        vector<Job *> u1, u2; // Initialize u1 and u2
        partition_jobs(jobs, completion_times[machine], u1, u2);

        long long min_process_time = LLONG_MAX;
        long long min_due_date = LLONG_MAX;
        for (Job* job : u1)
            min_process_time = min(min_process_time, job->ptime);
        for (Job* job : u2)
            min_due_date = min(min_due_date, job->dDate);

        vector<Job*> gamma_lambda;
        for (Job* job : u1) 
            if (job->ptime == min_process_time)
                gamma_lambda.push_back(job);
        for (Job* job : u2) 
            if (job->dDate == min_due_date) 
                gamma_lambda.push_back(job);

        pair<Job*, long long> min_mdd_job = find_min_mdd_job(gamma_lambda, completion_times[machine]);
        cantidate_jobs[machine] = min_mdd_job;
        min_delta = min(min_delta, min_mdd_job.second);
        max_delta = max(max_delta, min_mdd_job.second);
    }

    long long condition_rcl = min_delta + alpha * (max_delta - min_delta);
    vector<pair<Job *, int>> rcl_jobs;
    for (int machine = 0; machine < numMachines; machine++) {
        if (cantidate_jobs[machine].second <= condition_rcl) {
            rcl_jobs.push_back(make_pair(cantidate_jobs[machine].first, machine));
        }
    }
    return rcl_jobs;
}

/**
 * Constructs a schedule using a random greedy algorithm.
 * 
 * This function takes a list of jobs, the number of machines, and an alpha value as input.
 * It constructs a schedule by iteratively selecting a job and a machine based on a random greedy algorithm.
 * The algorithm uses a restricted candidate list (RCL) to select the job and machine combination.
 * The selected job is then scheduled on the selected machine, and its completion time is updated.
 * The job is removed from the list of unscheduled jobs.
 * This process continues until all jobs are scheduled.
 * 
 * @param jobs The list of jobs to be scheduled.
 * @param numMachines The number of machines available for scheduling.
 * @param alpha The alpha value used to calculate the condition for RCL.
 * @return A 2D vector representing the constructed schedule, where each inner vector represents a machine and contains the scheduled jobs.
 */
vector<vector<Job*>> random_greedy_construction(vector<Job*> jobs, int numMachines, float alpha) {
    vector<vector<Job*>> schedule(numMachines);
    
    vector<long long> completion_times(numMachines, 0);
    while (!jobs.empty()) {
        
        int selected_machine;
        Job* selected_job;
        vector<pair<Job *, int>> rcl_jobs = RCL(jobs, numMachines, completion_times, selected_machine, selected_job, alpha);
        int random_index = rand() % rcl_jobs.size();
        selected_job = rcl_jobs[random_index].first;
        selected_machine = rcl_jobs[random_index].second;

        schedule[selected_machine].push_back(selected_job);
        completion_times[selected_machine] += selected_job->ptime;

        // erase the job from the list of unscheduled jobs with its id
        for (int i = 0; i < jobs.size(); i++) {
            if (jobs[i]->id == selected_job->id) {
                jobs.erase(jobs.begin() + i);
                break;
            }
        }
    }

    return schedule;
}       

/**
 * Implements the GRASP (Greedy Randomized Adaptive Search Procedure) algorithm.
 * 
 * This algorithm generates a set of random solutions and iteratively improves them
 * by applying local search techniques. It returns a list of the best solutions found
 * at specific iterations along with the time taken to find each solution.
 * 
 * @param jobs The list of jobs to be scheduled.
 * @param numMachines The number of machines available for scheduling.
 * @param max_iters The maximum number of iterations to perform.
 * @param alpha The alpha value used to calculate the condition for RCL.
 * @return A vector of pairs, where each pair contains a solution and the time taken to find it.
 */
vector<pair<vector<vector<Job *>>, duration<double>>> grasp(vector<Job *> jobs, int numMachines, int max_iters, float alpha) {
    high_resolution_clock::time_point start, end;
    start = high_resolution_clock::now();
    // Generate a random solution
    vector<vector<Job*>> curr_solution = generate_random_solution(jobs, numMachines);
    long long curr_tt = total_tardiness(curr_solution);

    vector<pair<vector<vector<Job *>>, duration<double>>> best_solutions;

    unsigned int i = 0;
    while (i < max_iters) {
        vector<vector<Job*>> new_solution = random_greedy_construction(jobs, numMachines, alpha);
        new_solution = improve_solution_by_ls(new_solution, numMachines, 1000);

        long long new_tt = total_tardiness(new_solution);
        if (new_tt < curr_tt) {
            curr_solution = new_solution;
            curr_tt = new_tt;
        }

        i++;
        if (i == 30 || i == 60 || i == 100) {
            end = high_resolution_clock::now();
            duration<double> duration = duration_cast<chrono::duration<double>>(end - start);
            best_solutions.push_back(make_pair(curr_solution, duration));
        }
    }
    return best_solutions;
}