#ifndef GRASP_H
    #define GRASP_H

    #include <vector>
    #include "../header_files/Job.h"
    #include "../header_files/local_search.h"
    #include "../header_files/exact.h"
    #include "../header_files/heuristic.h"

    using namespace std;

    vector<pair<Job *, int>> RCL(vector<Job *> jobs, int numMachines, vector<long long> completion_times, int &selected_machine, Job *&selected_job, float alpha);
    vector<vector<Job*>> random_greedy_construction(vector<Job*> jobs, int numMachines, float alpha);
    vector<vector<Job *>> grasp(vector<Job *> jobs, int numMachines, int max_iters);

#endif