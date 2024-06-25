#include <algorithm>
#include <vector>
#include <climits>
#include "../header_files/evolution.h"
#include "../header_files/memetic.h"
#include "../header_files/local_search.h"

using namespace std;


int find_job_in_individual(Individual ind, int job_id) {
    for (int i = 0; i < ind.size(); i++) {
        if (ind[i].job->id == job_id) {
            return i;
        }
    }

    return -1;
}

bool equal_individuals(Individual ind1, Individual ind2) {

    if (ind1.size() != ind2.size()) {
        return false;
    }

    for (int i = 0; i < ind1.size(); i++) {
        if (ind1[i].job->id != ind2[i].job->id || ind1[i].machine != ind2[i].machine) {
            return false;
        }
    }

    return true;
}

Individual path_relinking(Individual ind, Individual target, int machine_count) {
    long long fitness_ind = fitness(get_fenotype(ind, machine_count));
    long long fitness_target = fitness(get_fenotype(target, machine_count));

    long long max_fitness = fitness_ind > fitness_target ? fitness_ind : fitness_target;
    Individual best_fit = fitness_ind > fitness_target ? ind : target;

    //Generate all intermediate points between ind and target
    for (int i = 0; i < target.size(); i++) {

        if (ind[i].job->id != target[i].job->id) {
            int job_idx = find_job_in_individual(ind, target[i].job->id);
            std::iter_swap(ind.begin() + i, ind.begin() + job_idx);

            //Update if current solution is better than the current one
            long long curr_fit = fitness(get_fenotype(ind, machine_count));
            if (curr_fit > max_fitness) {
                max_fitness = curr_fit;
                best_fit = ind;
            }
        }

        if (ind[i].machine != target[i].machine) {
            ind[i].machine = target[i].machine;

            //Update if current solution is better than the current one
            long long curr_fit = fitness(get_fenotype(ind, machine_count));
            if (curr_fit > max_fitness) {
                max_fitness = curr_fit;
                best_fit = ind;
            }
        }
    }

    return best_fit;
}

/**
 * Distance function for the Scatter Search algorithm. For each job ID
 * a distance point will be added if the individuals involved have the
 * job scheduled in different machines.
 */
int distance_function(Individual ind1, Individual ind2) {
    int distance = 0;

    for (Gene g : ind1) {
        int idx_in_ind2 = find_job_in_individual(ind2, g.job->id);
        if (g.machine != ind2[idx_in_ind2].machine) {
            distance += 1;
        }
    }

    return distance;
}

int minimum_distance_to_ref_set(Population ref_set, Individual pool_sample) {
    int min_distance = INT_MAX;

    for (Individual ref_set_member : ref_set) {
        int distance = distance_function(ref_set_member, pool_sample);
        if (distance < min_distance) {
            min_distance = distance;
        }
    }

    return min_distance;
}

/**
 * Generates the reference set from the solution pool. The first 10% of reference set
 * members will be included based on fitness, the remaining 90% will be decided based on 
 * diversity
 */
Population generate_reference_set(Population pool, int ref_set_size, int machine_count) {
    Population reference_set;

    //Sort candidate pool by fitness
    std::sort(pool.begin(), pool.end(), [machine_count](const Individual &i1, const Individual &i2) {
        return fitness(get_fenotype(i1, machine_count)) > fitness(get_fenotype(i2, machine_count));
    });

    //Get fitness-based members of the reference set
    int fitness_based_count = ref_set_size / 10;
    for (int i = 0; i < fitness_based_count; i++) {
        reference_set.push_back(pool[i]);
    }

    //Remove included members from the pool
    pool.erase(pool.begin(), pool.begin() + fitness_based_count);

    while (reference_set.size() < ref_set_size) {
        //Find the candidates seeing the closest reference set member for each pool member
        vector<int> min_distances;

        for (Individual pool_sample : pool) {
            min_distances.push_back(minimum_distance_to_ref_set(reference_set, pool_sample));
        }

        //Find the position of the individual with the maximum minimal distance to the reference set
        int max_distance = INT_MIN;
        int max_dist_elem_pos = -1;
        for (int i = 0; i < pool.size(); i++) {
            if (min_distances[i] > max_distance) {
                max_distance = min_distances[i];
                max_dist_elem_pos = i;
            }
        }

        //Add the chosen candidate to the reference set and remove it from the pool
        reference_set.push_back(pool[max_dist_elem_pos]);
        pool.erase(pool.begin() + max_dist_elem_pos);
    }

    return reference_set;
}

Fenotype scatter_search(vector<Job *> jobs, int machine_count, int max_pool_size, int ref_set_size, int parent_count, int max_iter, double path_relinking_ptg) {
    //Generate random initial population
    Population pool = generate_population(jobs, machine_count, max_pool_size);
    int generation = 0;

    //Apply brief local search on initial population to increase general quality
    for (int i = 0; i < pool.size(); i++) {
        Fenotype fen = get_fenotype(pool[i], machine_count);
        pool[i] = get_genotype(improve_solution_by_ls(fen, machine_count, 50), machine_count);
    }

    //Create initial reference set
    Population reference_set = generate_reference_set(pool, ref_set_size, machine_count);
    vector<long long> fitnesses(ref_set_size);

    do {
        // Compute fitness of all members of the reference set
        long long total_fitness_sum = 0;
        for (int i = 0; i < ref_set_size; i++) {
            fitnesses[i] = fitness(get_fenotype(reference_set[i], machine_count));
            total_fitness_sum += fitnesses[i];
        }

        //Generate the offspring of the reference set using multiple partially mapped crossover
        Population offspring;
        int offspring_count = 0;

        do {
            vector<Individual> parents = choose_multi_parents(5, reference_set, fitnesses, total_fitness_sum);
            vector<Individual> children = multi_partially_mapped_crossover(parents);

            for (Individual ch : children) {
                offspring.push_back(ch);
            }
        } while (offspring.size() < ref_set_size);

        //Apply Path Relinking to the specified percentage of the total pairs of offspring
        int pairs_to_check = (ref_set_size * ref_set_size) * path_relinking_ptg;
        int checked_pairs = 0;

        while (checked_pairs < pairs_to_check) {
            //Choose two offspring to do path relinking with
            int start_ind_idx = rand() % offspring.size();
            int target_ind_idx = rand() % offspring.size();

            Individual best_intermediate = path_relinking(offspring[start_ind_idx], offspring[target_ind_idx], machine_count);

            if (!equal_individuals(offspring[start_ind_idx], best_intermediate) && !equal_individuals(offspring[target_ind_idx], best_intermediate)) {
                offspring[start_ind_idx] = best_intermediate;
            }
            checked_pairs++;
        }

        //Create new pool with the union of the reference set and the offspring
        offspring.insert(offspring.end(), reference_set.begin(), reference_set.end());

        //Update the reference set
        reference_set = generate_reference_set(offspring, ref_set_size, machine_count);
    } while (fitness(get_fenotype(reference_set[0], machine_count)) != 0 && generation++ < max_iter);

    return get_fenotype(reference_set[0], machine_count);
}