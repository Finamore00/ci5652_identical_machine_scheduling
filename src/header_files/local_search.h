#ifndef LOCAL_SEARCH_H
    #define LOCAL_SEARCH_H
    #include <vector>
    #include "Job.h"

    using namespace std;
    
    vector<vector<Job*>> local_search(vector<Job*> jobs, int machine_count, unsigned int max_iter);
    vector<vector<Job*>> generate_random_solution(vector<Job*> jobs, int machine_count);
    vector<vector<Job*>> improve_solution_by_ls(vector<vector<Job*>> &curr_solution, int machine_count, unsigned int max_iter);

#endif