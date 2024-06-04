#ifndef VICINITIES_H
    #define VICINITIES_H

    #include "../header_files/Job.h"
    #include "../header_files/heuristic.h"
    #include <vector>
    #include <random>
    #include <algorithm>

    using namespace std;

    long long machine_tardiness(vector<Job*> machine_schedule);
    vector<vector<Job*>> shift_neighborhood(vector<vector<Job*>> schedule, int tardiest_machine);
    vector<vector<Job*>> switch_neighborhood(vector<vector<Job*>> schedule, int tardiest_machine);
    vector<vector<Job*>> direct_swap_neighborhood(vector<vector<Job*>> schedule, int tardiest_machine);
    vector<vector<Job*>> two_shift_neighborhood(vector<vector<Job*>> schedule, int tardiest_machine);
    vector<vector<Job*>> task_move_neighborhood(vector<vector<Job*>> schedule, int tardiest_machine);
    vector<vector<Job*>> generate_neighbor(vector<vector<Job*>> schedule, int tardiest_machine);
    int index_tardiest_machine(vector<vector<Job*>> schedule);

#endif