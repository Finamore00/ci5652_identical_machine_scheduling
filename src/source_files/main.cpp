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
#include "../header_files/grasp.h"
#include "../header_files/evolution.h"
#include "../header_files/simulated_annealing.h"
#include "../header_files/tabu_search.h"
#include "../header_files/ils.h"

using namespace std;
using namespace chrono;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Usage: ./PROY2 <path_to_benchmarks> <algorithm>" << endl;
        return -1;
    } 
    string path = argv[1];
    int algorithm = stoi(argv[2]);

    if (algorithm != 1 && algorithm != 2 && algorithm != 3 && algorithm != 4 && algorithm != 5) {
        cout << "Usage: ./PROY2 <path_to_benchmarks> <algorithm>" << endl;
        cout << " <algorithm> must be one of the following:" << endl;
        cout << "\t1: Iterative Local Search (ILS)" << endl;
        cout << "\t2: Tabu Search" << endl;
        cout << "\t3: Simulated Annealing" << endl;
        cout << "\t4: Greedy Randomized Adaptive Search Procedure (GRASP)" << endl;
        cout << "\t5: Genetic Algorithm" << endl;
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
            cout << "🔁 ILS" << endl;
            algorithm_name = "ILS";
            start = high_resolution_clock::now();
            schedule = ils(jobs, m, 1500,10,4,100,100);
            end = high_resolution_clock::now();
            break;
        case 2:
            cout << "🙊 Tabu Search" << endl;
            algorithm_name = "Tabu Search";
            start = high_resolution_clock::now();
            schedule = tabu_search(jobs, m, 14000);
            end = high_resolution_clock::now();
            break;
        case 3:
            cout << "❄️ Simulated Annealing" << endl;
            algorithm_name = "Simulated Annealing";
            start = high_resolution_clock::now();
            schedule = simulated_annealing(jobs, m, 2000.0, 0.90, 100, 2000);
            end = high_resolution_clock::now();
            break;
        case 4:
            cout << "🙌 GRASP" << endl;
            algorithm_name = "GRASP";
            {
                float alphas[] = {0.25, 0.5, 0.75, 1};
                int iters[] = {30, 60, 100};
                for (int j = 0; j < 3; j++)
                {
                    vector<pair<vector<vector<Job *>>, duration<double>>> best_solutions = grasp(jobs, m, 100, alphas[j]);
                    for (int i = 0; i < best_solutions.size(); i++)
                    {
                        cout << "🦙 GRASP Schedule with alpha = " << alphas[j] << " and iterations = " << iters[i] << endl;
                        print_schedule(best_solutions[i].first, m);
                        long long tardiness = total_tardiness(best_solutions[i].first);
                        cout << "\nTotal Tardiness: " << tardiness << endl;
                        cout << "Time taken by GRASP: " << best_solutions[i].second.count() << " seconds\n" << endl;
                    }
                }
            }
            break;
        case 5:
            cout << "🧬 Genetic Algorithm" << endl;
            algorithm_name = "Genetic Algorithm";
            start = high_resolution_clock::now();
            schedule = genetic_algorithm(jobs, m, 50, 0.1, 4000);
            end = high_resolution_clock::now();
            break;
        }
        if (algorithm == 4) continue;

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
            cout << "ILS: ";
            break;
        case 2:
            cout << "tabu search: ";
            break;
        case 3:
            cout << "simulated annealing: ";
            break;
        case 5:
            cout << "genetic algorithm: ";
            break;
        default:
            break;
        }
        cout << duration.count() << " seconds\n" << endl;
    }

    return 0;
}