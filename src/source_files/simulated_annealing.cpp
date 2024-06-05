#include "../header_files/Job.h"
#include "../header_files/heuristic.h"
#include "../header_files/vicinities.h"
#include "../header_files/exact.h"
#include <random>
#include <ctime>  

using namespace std;

/**
 * Generates a random double between 0.0 and 1.0 using a uniform distribution.
 *
 * @return A random double value between 0.0 and 1.0.
 * 
 * This function uses random library to generate a random double. The
 * random number generator is initialized with the current time to ensure different
 * sequences of random numbers for each run.
 */
double generate_random_double() {
    static default_random_engine generator(static_cast<unsigned>(time(0)));
    
    // Define a uniform distribution in the range [0.0, 1.0].
    uniform_real_distribution<double> distribution(0.0, 1.0);

    // Generate a random double using the defined distribution.
    return distribution(generator);
}

/**
 * Implements the Simulated Annealing algorithm for scheduling jobs across multiple machines.
 *
 * @param jobs A vector of job pointers that need to be scheduled.
 * @param machines The number of machines available for scheduling.
 * @param t0 The initial temperature for the simulated annealing algorithm.
 * @param t_step The factor by which the temperature is decreased in each iteration.
 * @param max_iter_t_step The maximum number of iterations at each temperature step.
 * @param max_iter The maximum number of iterations for the simulated annealing algorithm.
 * @return A vector of vectors where each inner vector represents the schedule of jobs for a machine, optimized to minimize tardiness.
 * 
 * The function starts by creating an initial schedule using the `mddScheduling` function and calculates its total tardiness.
 * It then iteratively improves the schedule by generating neighboring schedules and applying the simulated annealing strategy
 * to escape local optima. The best found schedule is returned after the maximum number of iterations.
 */
vector<vector<Job*>> simulated_annealing(vector<Job*> jobs, int machines, float t0, float t_step, int max_iter_t_step, int max_iter) {
    // Initialize the best and current schedules using the initial scheduling method.
    vector<vector<Job*>> best_schedule = mddScheduling(jobs, machines);
    vector<vector<Job*>> current_schedule, neighbor;
    current_schedule = best_schedule;
    
    // Initialize the temperature and the stopping criteria.
    int t = t0;
    double epsilon = 0.000001;
    int cnt = max_iter;
    
    // Calculate the total tardiness of the initial schedule.
    long long best_tardiness = total_tardiness(best_schedule);

    // Main loop for the Simulated Annealing algorithm.
    while (cnt--) {
        // Loop for the iterations at each temperature step.
        for(int i = 0; i < max_iter_t_step; i++) {
            // Generate a neighboring schedule.
            neighbor = generate_neighbor(current_schedule, index_tardiest_machine(current_schedule));
            long long neighbor_tardiness = total_tardiness(neighbor);
            
            // Calculate the change in tardiness.
            long long delta = neighbor_tardiness - total_tardiness(current_schedule);
            
            // If the neighboring schedule is better, accept it.
            if(delta <= 0) {
                current_schedule = neighbor;
                
                // Update the best schedule if the neighboring schedule is better.
                if (neighbor_tardiness < best_tardiness) {
                    best_schedule = neighbor;
                    best_tardiness = neighbor_tardiness;
                }
            } else {
                // Otherwise, accept the neighboring schedule with a probability.
                double r = generate_random_double();
                if (r < exp(-delta / t)) {
                    current_schedule = neighbor;
                }
            }
        }
        
        // Decrease the temperature.
        t *= t_step;
        
        // Reset the temperature if it goes below a threshold.
        if (t < epsilon) {
            t = t0;
        }
    }

    return best_schedule;
}
