#ifndef TABU_SEARCH_H
    #define TABU_SEARCH_H
    #include <vector>
    #include "Job.h"
    #include "Move.h"

    using namespace std;
    
    vector<vector<Job*>> tabu_search(vector<Job*> jobs, int machines, unsigned int max_iter, unsigned int max_grn_iter, unsigned int tabu_tenure);
#endif