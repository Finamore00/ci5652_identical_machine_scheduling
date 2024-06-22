#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include "../header_files/heuristic.h"
#include "../header_files/Job.h"
#include "../header_files/exact.h"
#include "../header_files/local_search.h"

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> dis(0.0, 1.0);

/**
 * Selects a machine based on the given machine times and a probability threshold.
 *
 * @param machine_times The vector of machine times.
 * @param q0 The probability threshold.
 * @return The index of the selected machine.
 */
int select_machine(const vector<long long>& machine_times, const double q0) {
    // Exploitation
    if (dis(gen) < q0) {
        // Select the machine with the minimum processing time
        return min_element(machine_times.begin(), machine_times.end()) - machine_times.begin();
    } else {
        // Exploration
        vector<double> probabilities(machine_times.size());
        // Calculate the probabilities for each machine
        double sum = 0.0;
        for (int i = 0; i < machine_times.size(); ++i) {
            probabilities[i] = 1.0 / machine_times[i];
            sum += probabilities[i];
        }
        // Select a machine based on the probabilities
        double r = dis(gen) * sum;
        for (int i = 0; i < machine_times.size(); ++i) {
            r -= probabilities[i];
            if (r <= 0) return i;
        }
        return machine_times.size() - 1;
    }
}

/**
 * Selects the machine with the minimum completion time for a given task.
 * 
 * @param machine_times A vector containing the completion times of each machine.
 * @param task          A pointer to the task for which the machine needs to be selected.
 * @return              The index of the machine with the minimum completion time.
 */
int machine_reselection(const vector<long long>& machine_times, Job* task) {
    // Initialize the minimum value and the machine index
    long long min_value = numeric_limits<long long>::max();
    int machine = -1;
    // Iterate over the machines to find the one with the minimum completion time
    for (int i = 0; i < machine_times.size(); ++i) {
        long long value = machine_times[i] + task->ptime - task->dDate;
        if (value < min_value) {
            min_value = value;
            machine = i;
        }
    }
    return machine;
}

/**
 * Selects a job to be scheduled on a given machine using Ant Colony Optimization (ACO) algorithm.
 *
 * @param jobs The list of available jobs.
 * @param machine The index of the machine on which the job will be scheduled.
 * @param scheduled A boolean vector indicating whether each job has been scheduled or not.
 * @param pheromone The pheromone matrix containing the pheromone levels between jobs and machines.
 * @param machine_times The current time spent on each machine.
 * @param alpha The weight parameter for the pheromone level in the selection process.
 * @param beta The weight parameter for the visibility (inverse of the machine-dependent delay) in the selection process.
 * @param q0 The probability of selecting the job with the highest value (exploitation) instead of using probabilities (exploration).
 * @return The index of the selected job.
 */
int select_job(const vector<Job*> jobs, int machine, 
               const vector<bool>& scheduled, 
               const vector<vector<double>>& pheromone, 
               vector<long long> machine_times,
               const double alpha, const double beta, 
               const double q0) {
    // Exploitation
    if (dis(gen) < q0) {
        double max_value = -1;
        int selected_job = -1;
        // Select the job with the highest value in terms of pheromone and visibility
        for (int i = 0; i < scheduled.size(); ++i) {
            if (!scheduled[i]) {
                double visibility = 1/mdd(machine_times[machine], jobs[i]);
                double value = pow(pheromone[i][machine], alpha) * pow(visibility, beta);
                if (value > max_value) {
                    max_value = value;
                    selected_job = i;
                }
            }
        }
        return selected_job;
    } else {
        // Exploration
        vector<double> probabilities(scheduled.size(), 0.0);
        double sum = 0.0;
        // Calculate the probabilities for each job
        for (int i = 0; i < scheduled.size(); ++i) {
            if (!scheduled[i]) {
                double visibility = 1/mdd(machine_times[machine], jobs[i]);
                probabilities[i] = pow(pheromone[i][machine], alpha) * pow(visibility, beta);
                sum += probabilities[i];
            }
        }
        // Select a job based on the probabilities
        double r = dis(gen) * sum;
        for (int i = 0; i < scheduled.size(); ++i) {
            if (!scheduled[i]) {
                r -= probabilities[i];
                if (r <= 0) return i;
            }
        }
        return scheduled.size() - 1;
    }
}

/**
 * Updates the local pheromone level for a specific job and machine.
 * 
 * This function updates the local pheromone level for a given job and machine
 * based on the number of ants, the maximum due date tardiness, and a local
 * evaporation rate.
 * 
 * @param pheromone The matrix representing the pheromone levels for each job and machine.
 * @param job The index of the job.
 * @param machine The index of the machine.
 * @param rho_local The local evaporation rate.
 * @param ants The number of ants.
 * @param mdd_tardiness The maximum due date tardiness.
 */
void local_pheromone_update(vector<vector<double>>& pheromone, int job, int machine, const double rho_local, int ants, long long mdd_tardiness) {
    double delta = 1.0 / mdd_tardiness;
    pheromone[job][machine] = (1 - rho_local) * pheromone[job][machine] + rho_local * delta;
}

/**
 * Updates the global pheromone matrix based on the best solution found.
 * 
 * @param pheromone The global pheromone matrix.
 * @param best_solution The best solution found.
 * @param best_tardiness The tardiness of the best solution.
 * @param rho_global The global pheromone evaporation rate.
 */
void global_pheromone_update(vector<vector<double>>& pheromone, 
                           const vector<vector<Job*>>& best_solution, 
                           long long best_tardiness, const double rho_global) {
    for (int i = 0; i < pheromone.size(); ++i) {
        for (int j = 0; j < pheromone[i].size(); ++j) {
            pheromone[i][j] = (1 - rho_global) * pheromone[i][j] + rho_global * (1.0 / best_tardiness);
        }
    }
}

/**
 * Applies the Ant Colony Optimization (ACO) algorithm to solve the identical machine scheduling problem.
 *
 * @param jobs The list of jobs to be scheduled.
 * @param machines The number of machines available for scheduling.
 * @param iterations The number of iterations to run the ACO algorithm.
 * @param ants The number of ants (solutions) to be generated in each iteration.
 * @param alpha The importance of the pheromone trail in the ant's decision-making process.
 * @param beta The importance of the heuristic information in the ant's decision-making process.
 * @param qm0 The probability of selecting the machine with the lowest completion time instead of using probabilities.
 * @param qj0 The probability of selecting the job with the highest value (exploitation) instead of using probabilities (exploration).
 * @param rho_local The local pheromone evaporation rate.
 * @param rho_global The global pheromone evaporation rate.
 * @return The best solution found by the ACO algorithm, represented as a vector of vectors of jobs.
 */
vector<vector<Job*>> ACO(vector<Job*> jobs, int machines, int iterations, int ants, 
                        int alpha, int beta, double qm0, double qj0, double rho_local, double rho_global) {
    // Initialize pheromone matrix
    vector<vector<double>> pheromone(jobs.size(), vector<double>(machines, 1.0));
    // Calculate the maximum due date tardiness using the MDD scheduling
    long long mdd_tardiness = total_tardiness(mddScheduling(jobs, machines)) + 1;
    
    // Initialize the best solution and its tardiness
    vector<vector<Job*>> best_solution(machines);
    long long best_tardiness = numeric_limits<long long>::max();

    // The termination condition is based on the number of iterations
    for (int iter = 0; iter < iterations; ++iter) {
        // Generate solutions for each ant
        for (int ant = 0; ant < ants; ++ant) {
            // Initialize the current solution and machine times
            vector<vector<Job*>> current_solution(machines);
            vector<long long> machine_times(machines, 0);
            vector<bool> scheduled(jobs.size(), false);
            // Construct the solution by selecting jobs for each machine
            for (int i = 0; i < jobs.size(); ++i) {
                // Select a machine and a job
                int machine = select_machine(machine_times, qm0);
                // Select a job to be scheduled on the selected machine using ACO
                int job = select_job(jobs, machine, scheduled, pheromone, machine_times, alpha, beta, qj0);
                // Reselect the machine if the machine is not the best choice for the job
                machine = machine_reselection(machine_times, jobs[job]);
                // Schedule the job on the selected machine
                current_solution[machine].push_back(jobs[job]);
                machine_times[machine] += jobs[job]->ptime;
                // Update the scheduled jobs and the pheromone level
                scheduled[job] = true;
                local_pheromone_update(pheromone, job, machine, rho_local, ant, mdd_tardiness);
            }
            // Improve the solution using local search
            current_solution = improve_solution_by_ls(current_solution, machines, 100);
            // Calculate the tardiness of the current solution
            long long current_tardiness = total_tardiness(current_solution);
            // Update the best solution if the current solution is better
            if (current_tardiness < best_tardiness) {
                best_solution = current_solution;
                best_tardiness = current_tardiness;
                if (best_tardiness == 0) return best_solution;
            }
        }
        // Update the global pheromone level based on the best solution
        global_pheromone_update(pheromone, best_solution, best_tardiness, rho_global);
    }
    return best_solution;
}
