#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include <iostream>
#include "../header_files/vicinities.h"
#include "../header_files/exact.h"
#include "../header_files/heuristic.h"
#include "../header_files/tabu_list.h"
using namespace std;



// Genera vecinos de una solución dada
vector<vector<Job*>> va(vector<vector<Job*>> machine_jobs, tabu_list& tl) {
    vector<vector<Job*>> best_solution, tabu_solution;
    long long best_tardiness = LLONG_MAX;
    long long current_tardiness = total_tardiness(machine_jobs);
    pair<int, int> tabu_move_1, tabu_move_2;

    // Find tardiest machine in the solution
    int tardiest_machine = -1;
    long long max_tardiness = LLONG_MIN;
    for (size_t i = 0; i < machine_jobs.size(); i++) {
        if (machine_tardiness(machine_jobs[i]) > max_tardiness) {
                max_tardiness = machine_tardiness(machine_jobs[i]);
                tardiest_machine = i;
        }
    }

    // Find earliess machine in the solution
    int earliess_machine = -1;
    long long min_tardiness = LLONG_MAX;
    for (size_t i = 0; i < machine_jobs.size(); i++) {
        if (machine_tardiness(machine_jobs[i]) < min_tardiness) {
                min_tardiness = machine_tardiness(machine_jobs[i]);
                earliess_machine = i;
        }
    }
    
    if (tardiest_machine != earliess_machine) {
        for (size_t j = 0; j < machine_jobs[tardiest_machine].size(); j++) {
            if(!tl.belongs(earliess_machine, j)) {
                for (size_t k = 0; k < machine_jobs[earliess_machine].size(); k++) {
                    if(!tl.belongs(tardiest_machine, j)) {
                        vector<vector<Job*>> new_solution = machine_jobs;

                        // Sacar el trabajo de la máquina i
                        Job* job_to_move = new_solution[tardiest_machine][j];
                        new_solution[tardiest_machine].erase(new_solution[tardiest_machine].begin() + j);

                        // Insertar el trabajo en la posición anterior al trabajo en la máquina tardía
                        new_solution[earliess_machine].insert(new_solution[earliess_machine].begin() + k, job_to_move);
                        if (total_tardiness(new_solution) < best_tardiness) {
                            best_solution = new_solution;
                            best_tardiness = total_tardiness(new_solution);
                            tabu_move_1 = make_pair(tardiest_machine, j);
                            tabu_move_2 = make_pair(earliess_machine, k);
                        }
                    }
                }
            }
        }
    }

    bool flag = false;
    for (size_t i = 0; i < tl.l.size(); i++) {
        if (tl.l[i].first != -1) {
            tabu_solution = direct_swap_neighborhood(machine_jobs, tl.l[i].first);
            if (total_tardiness(tabu_solution) < best_tardiness) {
                best_solution = tabu_solution;
                best_tardiness = total_tardiness(tabu_solution);
            }
        }
    }

    if(!flag){
        tl.add(tabu_move_1);
        tl.add(tabu_move_2);
    }

    return best_solution;
}


// Función principal de Búsqueda Tabú
vector<vector<Job*>> tabu_search(vector<Job*> jobs, int num_machines, int max_iterations) {
    vector<vector<Job*>> best_solution = mddScheduling(jobs, num_machines);
    long long best_tardiness = total_tardiness(best_solution);
    vector<vector<Job*>> current_solution = best_solution;
    long long current_tardiness = best_tardiness;
    int div = 0;
    if (best_tardiness == 0) {
        return best_solution;
    }

    tabu_list tl;

    for (int iter = 0; iter < max_iterations; ++iter) {
        current_solution = va(current_solution, tl);
        current_tardiness = total_tardiness(current_solution);
        if(current_tardiness < best_tardiness) {
            best_solution = current_solution;
            best_tardiness = best_tardiness;
            div = 0;
        } else {
            div++;
        }
        if (div == 3000) {
            for (size_t i = 0; i < 10; i++) {
                int earliess_machine = -1;
                long long min_tardiness = LLONG_MAX;
                for (size_t i = 0; i < current_solution.size(); i++) {
                    if (machine_tardiness(current_solution[i]) < min_tardiness) {
                            min_tardiness = machine_tardiness(current_solution[i]);
                            earliess_machine = i;
                    }
                }
                direct_swap_neighborhood(current_solution, earliess_machine);
            }  
            div = 0;
        }
    }

    return best_solution;
}