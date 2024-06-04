#ifndef ILS_H
    #define ILS_H
    #include "../header_files/utilities.h"
    #include "../header_files/vicinities.h"
    #include "../header_files/exact.h"
    #include "../header_files/heuristic.h"
    #include <vector>

    using namespace std;
    
    vector<vector<Job*>> ils(vector<Job*> jobs, int machines, unsigned int max_iter, unsigned int p0, unsigned int pmax, unsigned int rnamax,unsigned int itermax);
#endif