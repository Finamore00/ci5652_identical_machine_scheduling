#ifndef UTILITIES
    #define UTILITIES
    
    #include <string>
    #include <vector>
    #include <fstream>
    #include <iostream>
    #include <filesystem>
    #include "Job.h"

    using namespace std;

    vector<int> splitstring(string str);
    int read_fileinput(filesystem::path path, vector<Job*> &jobs, int &num_jobs, int &num_machines);
    void print_schedule(vector<vector<Job*>> schedule, int machine_count);

#endif