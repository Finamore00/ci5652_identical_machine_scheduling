#include "../header_files/Job.h"
#include "../header_files/heuristic.h"
#include "../header_files/vicinities.h"
#include "../header_files/exact.h"
#include <random>
#include <ctime>  

using namespace std;

double generate_random_double() {
    // Crear un generador de números aleatorios basado en el tiempo actual
    static default_random_engine generator(static_cast<unsigned>(time(0)));
    
    // Crear una distribución uniforme para números de punto flotante entre 0.0 y 1.0
    uniform_real_distribution<double> distribution(0.0, 1.0);

    // Generar un número aleatorio
    return distribution(generator);
}

vector<vector<Job*>> simulated_annealing(vector<Job*> jobs, int machines, float t0, int t_step, int max_iter_t_step, int max_iter) {
    vector<vector<Job*>> best_schedule = mddScheduling(jobs, machines);
    vector<vector<Job*>> current_schedule, neighbor;
    current_schedule = best_schedule;
    int t = t0;
    double epsilon = 0.000001;
    int cnt = max_iter;
    long long best_tardiness = total_tardiness(best_schedule);

    while (cnt--) {
        for(int i = 0; i < max_iter_t_step; i++) {
            neighbor = generate_neighbor(current_schedule, index_tardiest_machine(current_schedule));
            long long neighbor_tardiness = total_tardiness(neighbor);
            long long delta = neighbor_tardiness - total_tardiness(current_schedule);
            if(delta <= 0) {
                current_schedule = neighbor;
                if (neighbor_tardiness < best_tardiness) {
                    best_schedule = neighbor;
                    best_tardiness = neighbor_tardiness;
                } else {
                    double r = generate_random_double();
                    if (r < exp(-delta / t)) {
                        current_schedule = neighbor;
                    }
                }
            }
        }
        t *= t_step;
        if (t < epsilon) {
            t = t0;
        }
    }

    return best_schedule;
}