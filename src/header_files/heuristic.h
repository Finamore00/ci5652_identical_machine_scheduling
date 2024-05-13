#include "Job.h"
#include <vector>

#ifndef HEURISTIC_H
    #define HEURISTIC_H

    using namespace std;

    long long mdd(long long processed, Job task);
    void partition_jobs(vector<Job> unscheduled_jobs, long long processed, vector<Job> &u1, vector<Job> &u2);
    pair<Job, long long> find_min_mdd_job(vector<Job> unscheduled_jobs, long long processed);
    vector<vector<Job>> mddScheduling(vector<Job> jobs, int numMachines);

#endif