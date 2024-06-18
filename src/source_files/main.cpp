#include <iostream>
#include <filesystem>
#include <vector>
#include <climits>
#include <utility>
#include <string>
#include <chrono>
#include "../header_files/Job.h"
#include "../header_files/utilities.h"
#include "../header_files/exact.h"
#include "../header_files/memetic.h"

using namespace std;
using namespace chrono;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Usage: ./PROY3 <path_to_benchmarks> <algorithm>" << endl;
        return -1;
    } 
    string path = argv[1];
    int algorithm = stoi(argv[2]);

    if (algorithm != 1 && algorithm != 2 && algorithm != 3) {
        cout << "Usage: ./PROY3 <path_to_benchmarks> <algorithm>" << endl;
        cout << " <algorithm> must be one of the following:" << endl;
        cout << "\t1: Memetic Algorithm" << endl;
        cout << "\t2: Tabu Search" << endl;
        cout << "\t3: Simulated Annealing" << endl;
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
            cout << "😂😝 Memetic" << endl;
            algorithm_name = "Memetic";
            start = high_resolution_clock::now();
            schedule = memetic_algorithm(jobs, m, 500, 0.05, 10, 3);
            end = high_resolution_clock::now();
            break;
        case 2:
            cout << "🙊 Tabu Search" << endl;
            algorithm_name = "Tabu Search";
            start = high_resolution_clock::now();
            // schedule = tabu_search(jobs, m, 10000, 100, 7);
            end = high_resolution_clock::now();
            break;
        case 3:
            cout << "❄️ Simulated Annealing" << endl;
            algorithm_name = "Simulated Annealing";
            start = high_resolution_clock::now();
            // schedule = simulated_annealing(jobs, m, 1500.0, 0.85, 100, 1000);
            end = high_resolution_clock::now();
            break;
        }

        duration<double> duration = duration_cast<chrono::duration<double>>(end - start);

        cout << algorithm_name << " Schedule" << endl;
        print_schedule(schedule, m);

        // calculate total tardiness
        long long tardiness = total_tardiness(schedule);
        cout << "\nTotal Tardiness: " << tardiness << endl;

        cout << "Time taken by ";
        switch (algorithm)
        {
        case 1:
            cout << "memetic algorithm: ";
            break;
        case 2:
            cout << "tabu search: ";
            break;
        case 3:
            cout << "simulated annealing: ";
            break;
        default:
            break;
        }
        cout << duration.count() << " seconds\n" << endl;
    }

    return 0;
}