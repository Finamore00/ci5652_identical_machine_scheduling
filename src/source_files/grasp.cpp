#include <iostream>
#include <vector>
#include "../header_files/Job.h"
#include "../header_files/local_search.h"
#include "../header_files/exact.h"
#include "../header_files/heuristic.h"
using namespace std;

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


vector<vector<Job*>> grasp(vector<Job *> jobs, int numMachines, int max_iters) {
    vector<vector<Job*>> curr_solution = generate_random_solution(jobs, numMachines);
    long long curr_tt = total_tardiness(curr_solution);
    while(max_iters-- && curr_tt > 0) {
        // alpha between 0 and 1
        float alpha = (rand() % 100) / 100.0;
        vector<vector<Job*>> new_solution = random_greedy_construction(jobs, numMachines, alpha);
        new_solution = improve_solution_by_ls(new_solution, numMachines, 65000);

        long long new_tt = total_tardiness(new_solution);
        if (new_tt < curr_tt) {
            curr_solution = new_solution;
            curr_tt = new_tt;
        }
    }
    return curr_solution;
}