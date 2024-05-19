#include <iostream>
#include <vector>
#include <climits>
#include <utility>
#include "../header_files/Job.h"
#include "../header_files/utilities.h"

using namespace std;

/* 
    Calculate the minimum due date (MDD) for a given task.
    Args:
        - C is the completion time of the previous task
        - p is the processing time of the current task
        - d is the due date of the current task
    Returns:
        MDD = max{C + p, d}
*/
long long mdd(long long processed, Job *task) {
    return max(processed + task->ptime, task->dDate);
}


/*
    Partition unscheduled jobs into two sets for a machine based on due dates.
    Args:
        - unscheduled_jobs is a list of unscheduled jobs
        - processed is the sum of processing times of the jobs that have already been scheduled on the machine
        - u1 contains jobs that cannot be completed by their due date
        - u2 contains jobs that can be completed by their due date

*/
void partition_jobs(vector<Job *> unscheduled_jobs, long long processed, vector<Job *>& u1, vector<Job *>& u2) {
    for (int i = 0; i < unscheduled_jobs.size(); i++)
        if (processed + unscheduled_jobs[i]->ptime > unscheduled_jobs[i]->dDate)
            u1.push_back(unscheduled_jobs[i]);
        else
            u2.push_back(unscheduled_jobs[i]);
}

/*
    For a machine, find the job with the minimum MDD value from the unscheduled jobs.
    Args:
        - unscheduled_jobs is a list of unscheduled jobs
        - processed is the sum of processing times of the jobs that have already been scheduled on the machine
    Returns:
        - min_mdd_job is the job with the minimum MDD value
        - min_mdd is the minimum MDD value

*/
pair<Job*, long long> find_min_mdd_job(vector<Job*> unscheduled_jobs, long long processed) {
    long long min_mdd = LLONG_MAX; 
    Job* min_mdd_job;

    for (int i = 0; i < unscheduled_jobs.size(); i++) {
        int mdd2 = mdd(processed, unscheduled_jobs[i]);
        if (mdd2 < min_mdd) {
            min_mdd = mdd2;
            min_mdd_job = unscheduled_jobs[i];
        }
    }

    return make_pair(min_mdd_job, min_mdd);
}

/*
    Find the machine with the minimum delta value.
    Args:
        - jobs is a list of all unscheduled jobs
        - numMachines is the number of machines
        - completion_times is a list of completion times for all machines
        - min_delta_machine is the machine with the minimum delta value
        - min_delta_job is the job with the minimum delta value
*/
void find_gl(vector<Job *> jobs, int numMachines, vector<long long> completion_times, int &min_delta_machine, Job* &min_delta_job) {
    long long min_delta = LLONG_MAX;
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
        if (min_mdd_job.second < min_delta) {
            min_delta = min_mdd_job.second;
            min_delta_machine = machine;
            min_delta_job = min_mdd_job.first;
        }
    }
}

/*
    Implementation of the MDD algorithm for the TT (Total Tardiness) problem with multiple identical machines.
    Args:
        - jobs is a list of all jobs
        - numMachines is the number of machines
    Returns:
        - schedules is a list of schedules for each machine

*/
vector<vector<Job*>> mddScheduling(vector<Job *> jobs, int numMachines) {
    vector<long long> completion_times(numMachines, 0);  // Initialize completion times for all machines
    vector<vector<Job *>> schedules(numMachines);  // Initialize schedules for all machines

    // While there are unscheduled jobs
    while (!jobs.empty()) {
        
        int min_delta_machine = -1;
        Job* min_delta_job;

        // find the job with the minimum MDD value from the unscheduled jobs
        find_gl(jobs, numMachines, completion_times,  min_delta_machine, min_delta_job); 

        // schedule the job with the minimum MDD value on the machine with the minimum delta
        schedules[min_delta_machine].push_back(min_delta_job);
        // update the completion time of the machine
        completion_times[min_delta_machine] += min_delta_job->ptime;

        // erase the job from the list of unscheduled jobs with its id
        for (int i = 0; i < jobs.size(); i++) {
            if (jobs[i]->id == min_delta_job->id) {
                jobs.erase(jobs.begin() + i);
                break;
            }
        }
    }
    return schedules;
} 

