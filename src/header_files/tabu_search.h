#ifndef TABU_SEARCH_H
    #define TABU_SEARCH_H
    #include <vector>
    #include "Job.h"
    #include "tabu_list.h"

    using namespace std;
    
    vector<vector<Job*>> va(vector<vector<Job*>> machine_jobs, tabu_list& tl);
    vector<vector<Job*>> tabu_search(vector<Job*> jobs, int num_machines, int max_iterations);
#endif