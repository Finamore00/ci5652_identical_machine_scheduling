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
    if (argc != 3) {
        cout << "Usage: ./LocalSearch <path_to_benchmarks> <algorithm>" << endl;
        return -1;
    } 
    string path = argv[1];
    int algorithm = stoi(argv[2]);

    if (algorithm != 1 && algorithm != 2 && algorithm != 3) {
        cout << "Usage: ./LocalSearch <path_to_benchmarks> <algorithm>" << endl;
        cout << " <algorithm> must be one of the following:" << endl;
        cout << "\t1: Heuristic Algorithm" << endl;
        cout << "\t2: Local Search Algorithm" << endl;
        cout << "\t3: Exact Algorithm" << endl;
        return -1;
    }

    auto now = high_resolution_clock::now();
	auto timeMillis = duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
	srand(timeMillis);

    for (const auto & entry : filesystem::directory_iterator(path)) {
        // store the information of the file
        int n, m;
        vector<Job*> jobs;
        int flag = read_fileinput(entry.path(), jobs, n, m);

        if (flag == -1) {
            cout << "Error reading the file " << entry.path() << endl;
            return -1;
        }
        
        high_resolution_clock::time_point start, end;
        vector<vector<Job*>> schedule;
        string algorithm_name;

        switch (algorithm)
        {
        case 1:
            cout << "❤️ Heuristic Algorithm" << endl;
            algorithm_name = "Heuristic";
            start = high_resolution_clock::now();
            schedule = mddScheduling(jobs, m);
            end = high_resolution_clock::now();
            break;
        case 2:
            cout << "✨ Local Search Algorithm" << endl;
            algorithm_name = "Local Search";
            start = high_resolution_clock::now();
            schedule = local_search(jobs, m, 65000);
            end = high_resolution_clock::now();
            break;
        case 3:
            cout << "🙈 Exact Algorithm" << endl;
            algorithm_name = "Exact";
            start = high_resolution_clock::now();
            schedule = exact_solution(jobs, m);
            end = high_resolution_clock::now();
            break;
        }
        duration<double> duration = duration_cast<chrono::duration<double>>(end - start);

        cout << algorithm_name << " Schedule" << endl;
        print_schedule(schedule, m);

        // calculate total tardiness
        long long tardiness = total_tardiness(schedule);
        cout << "\nTotal Tardiness: " << tardiness << endl;
        cout << "Time taken by local search: " << duration.count() << " seconds\n" << endl;
    }

    return 0;
}