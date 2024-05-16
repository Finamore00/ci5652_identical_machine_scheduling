#ifndef VICINITIES_H
    #define VICINITIES_H

    #include "../header_files/Job.h"
    #include "../header_files/vicinities.h"
    #include <vector>
    #include <climits>
    #include <algorithm>

    using namespace std;

    pair<long long, vector<vector<Job>>> backtrack(vector<Job> jobs, vector<vector<Job>> schedules, long long mask);
    pair<long long, vector<vector<Job>>>exact_solution(vector<Job> jobs, int machines);

#endif