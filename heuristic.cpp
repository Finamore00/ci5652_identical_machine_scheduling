#include <iostream>
#include <vector>
#include <climits>
#include <utility>
#include <string>
#include <filesystem>
#include "Job.h"
#include "utilities.h"

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
long long mdd(long long processed, Job task) {
    return max(processed + task.ptime, task.dDate);
}


/*
    Partition unscheduled jobs into two sets for a machine based on due dates.
    Args:
        - unscheduled_jobs is a list of unscheduled jobs
        - processed is the sum of processing times of the jobs that have already been scheduled on the machine
        - u1 contains jobs that cannot be completed by their due date
        - u2 contains jobs that can be completed by their due date

*/
void partition_jobs(vector<Job> unscheduled_jobs, long long processed, vector<Job>& u1, vector<Job>& u2) {
    for (int i = 0; i < unscheduled_jobs.size(); i++)
        if (processed + unscheduled_jobs[i].ptime > unscheduled_jobs[i].dDate)
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
pair<Job, long long> find_min_mdd_job(vector<Job> unscheduled_jobs, long long processed) {
    /*
        Find the job with the minimum MDD value from the unscheduled jobs.
    */
    long long min_mdd = LLONG_MAX; 
    Job min_mdd_job;

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
    Implementation of the MDD algorithm for the TT (Total Tardiness) problem with multiple identical machines.
    Args:
        - jobs is a list of all jobs
        - numMachines is the number of machines
    Returns:
        - schedules is a list of schedules for each machine

*/
vector<vector<Job>> mddScheduling(vector<Job> jobs, int numMachines) {
    vector<long long> completion_times(numMachines, 0);  // Initialize completion times for all machines

    vector<vector<Job>> schedules(numMachines);  // Initialize schedules for all machines

    // While there are unscheduled jobs
    while (!jobs.empty()) {
        long long min_delta = LLONG_MAX;
        int min_delta_machine = -1;
        Job min_delta_job;

        for (int machine = 0; machine < numMachines; machine++) {
            vector<Job> u1, u2; // Initialize u1 and u2
            partition_jobs(jobs, completion_times[machine], u1, u2);

            // find the minimum processing time and due date for u1 and u2, respectively
            long long min_process_time = LLONG_MAX;
            long long min_due_date = LLONG_MAX;

            for (Job job : u1)
                min_process_time = min(min_process_time, job.ptime);

            for (Job job : u2)
                min_due_date = min(min_due_date, job.dDate);

            vector<Job> gamma_lambda;
            for (Job job : u1) 
                if (job.ptime == min_process_time)
                    gamma_lambda.push_back(job);

            for (Job job : u2) 
                if (job.dDate == min_due_date) 
                    gamma_lambda.push_back(job);
            
            // find the job with the minimum MDD value from gamma and lambda
            pair<Job, long long> min_mdd_job = find_min_mdd_job(gamma_lambda, completion_times[machine]);

            if (min_mdd_job.second < min_delta) {
                min_delta = min_mdd_job.second;
                min_delta_machine = machine;
                min_delta_job = min_mdd_job.first;
            }
            
        }

        // schedule the job with the minimum MDD value on the machine with the minimum delta
        schedules[min_delta_machine].push_back(min_delta_job);
        // update the completion time of the machine
        completion_times[min_delta_machine] += min_delta_job.ptime;

        // erase the job from the list of unscheduled jobs with its id
        for (int i = 0; i < jobs.size(); i++) {
            if (jobs[i].id == min_delta_job.id) {
                jobs.erase(jobs.begin() + i);
                break;
            }
        }
    }

    return schedules;
} 

int main() {
    // for each file of benchmarks (n=20, m=2), read the file and run the MDD algorithm
    string path = "benchmarks/n=20/m=2/";
    
    for (const auto & entry : filesystem::directory_iterator(path)) {
        // store the information of the file
        int n, m;
        vector<Job> jobs;
        int flag = read_fileinput(entry.path(), jobs, n, m);

        if (flag == -1) {
            cout << "Error reading the file " << entry.path() << endl;
            return -1;
        }

        // Run the MDD algorithm for the TT problem with n jobs and m machines
        vector<vector<Job>> schedules = mddScheduling(jobs, m);

    
        // print the schedule
        for (int i = 0; i < m; i++) {
            cout << "Machine " << i + 1 << ": ";
            for (Job job : schedules[i]) {
                cout << job.id << " ";
            }
            cout << endl;
        }
        
        // calculate total tardiness
        long long total_tardiness = 0;
        for (int i = 0; i < m; i++) {
            long long completion_time = 0;
            for (Job job : schedules[i]) {
                completion_time += job.ptime;
                long long tardiness = max(0LL, completion_time - job.dDate);
                total_tardiness += tardiness;
            }
        } 

        cout << "Total Tardiness: " << total_tardiness << endl;

    }
    
    return 0;
}