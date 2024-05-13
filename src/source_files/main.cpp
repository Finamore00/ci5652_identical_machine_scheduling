#include <iostream>
#include <vector>
#include <climits>
#include <utility>
#include <string>
#include <filesystem>
#include "../header_files/Job.h"
#include "../header_files/utilities.h"
#include "../header_files/heuristic.h"

int main() {
    // for each file of benchmarks (n=20, m=2), read the file and run the MDD algorithm
    string path = "benchmarks/n=20/m=2/";
    
    for (const auto & entry : filesystem::directory_iterator(path)) {
        // store the information of the file
        int n, m;
        vector<Job> jobs;
        int flag = read_fileinput(entry.path(), jobs, n, m);

        if (flag == -1) {
            cout << "Error reading the file " << entry.path() << endl;
            return -1;
        }

        // Run the MDD algorithm for the TT problem with n jobs and m machines
        vector<vector<Job>> schedules = mddScheduling(jobs, m);

    
        // print the schedule
        for (int i = 0; i < m; i++) {
            cout << "Machine " << i + 1 << ": ";
            for (Job job : schedules[i]) {
                cout << job.id << " ";
            }
            cout << endl;
        }
        
        // calculate total tardiness
        long long total_tardiness = 0;
        for (int i = 0; i < m; i++) {
            long long completion_time = 0;
            for (Job job : schedules[i]) {
                completion_time += job.ptime;
                long long tardiness = max(0LL, completion_time - job.dDate);
                total_tardiness += tardiness;
            }
        } 

        cout << "Total Tardiness: " << total_tardiness << endl;

    }
    
    return 0;
}