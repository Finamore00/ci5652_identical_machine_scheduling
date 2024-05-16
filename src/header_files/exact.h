#ifndef VICINITIES_H
    #define VICINITIES_H

    #include "../header_files/Job.h"
    #include <vector>
    #include <climits>
    #include <algorithm>

    using namespace std;

    vector<vector<Job>> backtrack(vector<Job> jobs, vector<vector<Job>> schedules, long long mask);
    vector<vector<Job>> exact_solution(vector<Job> jobs, int machines);

#endif