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
#include "../header_files/aco.h"
#include "../header_files/scattered.h"

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
        cout << "\t3: Ant Colony Optimization" << endl;
        return -1;
    }

    auto now = high_resolution_clock::now();
	auto timeMillis = duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
	srand(timeMillis);

    vector<filesystem::directory_entry> entries;

    for (const auto & entry : filesystem::directory_iterator(path)) {
        entries.push_back(entry);
    }

    sort(entries.begin(), entries.end(), [](const filesystem::directory_entry &a, const filesystem::directory_entry &b) {
        return a.path().filename().string() < b.path().filename().string();
    });

    for (const auto & entry : entries) {
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
            // schedule = memetic_algorithm(jobs, m, 500, 0.05, 20, 3, 2, 0.75, false);
            // add more parents to recombination
            // schedule = memetic_algorithm(jobs, m, 500, 0.05, 20, 10, 2, 0.75, false);
            // increase the frequency of local search 
            // schedule = memetic_algorithm(jobs, m, 500, 0.05, 20, 10, 1, 0.75, false);
            // increase rate of local search 
            // schedule = memetic_algorithm(jobs, m, 500, 0.05, 20, 10, 1, 1, false);
            // random frequency of local search and decrease the population
            // schedule = memetic_algorithm(jobs, m, 250, 0.05, 20, 5, 2, 1, true);
            // no random frequency of local search and decrease the population
            schedule = memetic_algorithm(jobs, m, 250, 0.05, 20, 5, 2, 1, false);
            end = high_resolution_clock::now();
            break;
        case 2:
            cout << "🦀 Scattered Search" << endl;
            algorithm_name = "Scattered Search";
            start = high_resolution_clock::now();
            //Initial run
            // schedule = scatter_search(jobs, m, 40, 20, 4, 20, 0.3);
            //Increase Iterations
            // schedule = scatter_search(jobs, m, 40, 20, 4, 30, 0.3);
            //Extra parent
            // schedule = scatter_search(jobs, m, 40, 20, 5, 30, 0.3);
            //Bigger reference set and max pool size
            // schedule = scatter_search(jobs, m, 60, 30, 5, 20, 0.3);
            //Increase iterations
            // schedule = scatter_search(jobs, m, 60, 30, 5, 30, 0.3);
            //Add extra parent
            schedule = scatter_search(jobs, m, 60, 30, 6, 20, 0.3);

            end = high_resolution_clock::now();
            break;
        case 3:
            cout << "🐜 Ant Colony Optimization" << endl;
            algorithm_name = "Ant Colony Optimization";
            start = high_resolution_clock::now();
            // schedule = ACO(jobs, m, 250, 20, 1, 3, 0.9, 0.9, 0.01, 0.01);
            // add more ants to the algorithm
            // schedule = ACO(jobs, m, 250, 50, 1, 3, 0.9, 0.9, 0.01, 0.01);
            // increase the rate of pheromone evaporation
            // schedule = ACO(jobs, m, 250, 20, 1, 3, 0.9, 0.9, 0.1, 0.1);
            // Change the alpha and beta values for analizing the impact of the heuristic
            // schedule = ACO(jobs, m, 250, 20, 3, 1, 0.9, 0.9, 0.01, 0.01);
            // Change the probability of exploring or exploiting
            // schedule = ACO(jobs, m, 250, 20, 1, 3, 0.7, 0.7, 0.01, 0.01);
            // Reduce the number of iterations
            schedule = ACO(jobs, m, 100, 20, 1, 3, 0.9, 0.9, 0.01, 0.01);
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
            cout << "scattered_search: ";
            break;
        case 3:
            cout << "ant colony optimization: ";
            break;
        default:
            break;
        }
        cout << duration.count() << " seconds\n" << endl;
    }

    return 0;
}