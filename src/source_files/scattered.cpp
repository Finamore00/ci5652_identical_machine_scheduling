#include <algorithm>
#include <vector>
#include "../header_files/evolution.h"
#include "../header_files/memetic.h"

using namespace std;

void new_print_genotype(Individual i) {
    cout << "[";
    for (Gene g : i) {
        cout << "<" << g.job->id << ", " << g.machine << ">, ";
    }

    cout << "]" << endl;
}

int find_job_in_individual(Individual ind, int job_id) {
    for (int i = 0; i < ind.size(); i++) {
        if (ind[i].job->id == job_id) {
            return i;
        }
    }

    return -1;
}

vector<Individual> path_relinking(Individual ind, Individual target) {
    vector<Individual> intermediate_path;
    intermediate_path.push_back(ind);

    //For each job in the target individual
    for (int i = 0; i < target.size(); i++) {

        if (ind[i].job->id != target[i].job->id) {
            int job_idx = find_job_in_individual(ind, target[i].job->id);
            std::iter_swap(ind.begin() + i, ind.begin() + job_idx);
            intermediate_path.push_back(ind);
        }

        if (ind[i].machine != target[i].machine) {
            ind[i].machine = target[i].machine;
            intermediate_path.push_back(ind);
        }
    }

    intermediate_path.push_back(target);

    return intermediate_path;
}

Fenotype scatter_search(vector<Job*> jobs, 
    int machine_count,
    int population_size,
    int reference_set_size,
    int parent_count,
    int max_iter,
    double path_relinking_ptg) { 
    
    //Generate initial population
    Population pop = generate_population(jobs, machine_count, population_size);

    cout << "Testing Path Relinking" << endl;
    cout << "Starting Point:" << endl;
    new_print_genotype(pop[0]);
    cout << "Target Individual" << endl;
    new_print_genotype(pop[1]);

    vector<Individual> path = path_relinking(pop[0], pop[1]);

    cout << "Path:" << endl;
    for (Individual i : path) {
        new_print_genotype(i);
    }

    return get_fenotype(pop[0], machine_count);
}