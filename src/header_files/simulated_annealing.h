#ifndef SIMULATED_ANNEALING_H
    #define SIMULATED_ANNEALING_H
    #include "../header_files/Job.h"
    #include "../header_files/heuristic.h"
    #include "../header_files/vicinities.h"
    #include "../header_files/exact.h"
    #include <vector>

    using namespace std;
    
    vector<vector<Job*>> simulated_annealing(vector<Job*> jobs, int machines, float T, int Tstep, int MaxP, int MaxIt, int MaxN, int MaxNe);
#endif