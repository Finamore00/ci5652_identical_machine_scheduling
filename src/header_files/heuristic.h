#ifndef HEURISTIC_H
    #define HEURISTIC_H

    #include "Job.h"
    #include <vector>
    #include <iostream>
    #include <vector>
    #include <algorithm>
    #include <climits>
    #include <utility>
    #include "../header_files/Job.h"
    #include "../header_files/utilities.h"

    using namespace std;

    long long mdd(long long processed, Job* task);
    void partition_jobs(vector<Job*> unscheduled_jobs, long long processed, vector<Job*> &u1, vector<Job*> &u2);
    pair<Job*, long long> find_min_mdd_job(vector<Job*> unscheduled_jobs, long long processed);
    vector<vector<Job*>> mddScheduling(vector<Job*> jobs, int numMachines);

#endif