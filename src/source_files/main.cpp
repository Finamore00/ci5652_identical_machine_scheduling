#include <iostream>
#include <vector>
#include <climits>
#include <utility>
#include <string>
#include <filesystem>
#include <chrono>
#include "../header_files/Job.h"
#include "../header_files/utilities.h"
#include "../header_files/heuristic.h"
#include "../header_files/exact.h"
#include "../header_files/local_search.h"

using namespace std::chrono;

int main(int argc, char *argv[]) {
    // for each file of benchmarks (n=20, m=2), read the file and run the MDD algorithm
    string path = argv[1];
    
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

        // auto start = high_resolution_clock::now();
        // vector<vector<Job>> heuristic_schedule = mddScheduling(jobs, m);
        // auto end = high_resolution_clock::now();
        // std::chrono::duration<double>  duration_h = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        
        // start = high_resolution_clock::now(); 
        // vector<vector<Job>> exact_schedules = exact_solution(jobs, m);
        // end = high_resolution_clock::now();
        // std::chrono::duration<double>  duration_e = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

        auto start = high_resolution_clock::now();
        vector<vector<Job>> local_search_schedule = local_search(jobs, m, 25000);
        auto end = high_resolution_clock::now();
        std::chrono::duration<double> duration_ls = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);


        // cout << "❤️ Heuristic Schedule" << endl;
        // print_schedule(heuristic_schedule, m);

        // calculate total tardiness
        // long long total_tardiness_heuristic = total_tardiness(heuristic_schedule);
        // cout << "\nTotal Tardiness: " << total_tardiness_heuristic << endl;
        // cout << "Time taken by heuristic: " << duration_h.count() << " seconds\n" << endl;

        // cout << "🙈 Exact Schedule" << endl;
        // print_schedule(exact_schedules, m);

        // calculate total tardiness
        // long long exact_total_tardiness = total_tardiness(exact_schedules);
        // cout << "\nTotal Tardiness: " << exact_total_tardiness << endl;
        // cout << "Time taken by exact algorithm: " << duration_e.count() << " seconds\n" << endl;

        cout << "✨ Local Search Schedule" << endl;
        print_schedule(local_search_schedule, m);

        // calculate total tardiness
        long long local_search_total_tardiness = total_tardiness(local_search_schedule);
        cout << "\nTotal Tardiness: " << local_search_total_tardiness << endl;
        cout << "Time taken by local search: " << duration_ls.count() << " seconds\n\n" << endl;
    }

    return 0;
}