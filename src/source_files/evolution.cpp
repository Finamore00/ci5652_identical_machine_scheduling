#include "../header_files/Job.h"
#include "../header_files/vicinities.h"
#include "../header_files/local_search.h"
#include "../header_files/exact.h"
#include <vector>
#include <unordered_map>
#include <random>

using namespace std;

typedef struct {
    Job *job;
    int machine;
} Gene;

//Useful aliases
typedef vector<vector<Gene>> Population;
typedef vector<Gene> Individual;
typedef vector<vector<Job*>> Fenotype;

void print_genotype(Individual ind) {
    for (Gene g : ind) {
        cout << "Machine " << g.machine + 1 << ": " << "Job " << g.job->id << endl;
    }
}

Individual generate_individual(vector<Job*> jobs, int machine_count) {
    vector<int> job_ids(jobs.size());
    for (int i = 0; i < jobs.size(); i++) {
        job_ids[i] = i; //Making id list to pick from
    }

    Individual ind;
    for (int i = 0; i < jobs.size(); i++) {
        int job_id_pos = rand() % job_ids.size();

        Gene new_ind = {jobs[job_ids[job_id_pos]], rand() % machine_count}; //Insert the chosen job in a random machine
        ind.push_back(new_ind);

        //Remove the chosen id from the id list so it won't be chosen again
        job_ids.erase(job_ids.begin() + job_id_pos);
    }

    return ind;
}

Population generate_population(vector<Job*> jobs, int machine_count, int population_size) {
    Population population(population_size);

    for (int i = 0; i < population_size; i++) {
        population[i] = generate_individual(jobs, machine_count);
    }

    return population;
}

Fenotype get_fenotype(Individual individual, int machine_count) {
    Fenotype fen(machine_count);

    for (Gene g : individual) {
        fen[g.machine].push_back(g.job);
    }
    return fen;
}

long long fitness(Fenotype fen) {
    return -total_tardiness(fen);
}

void mutate(Individual &ind, int machine_count, float mutation_rate) {

    for (Gene g : ind) {
        float prob = (float)rand() / (float)(RAND_MAX);

        if (prob < mutation_rate) {
            g.machine = rand() % machine_count; //To mutate assign the job to a different machine
        }
    }
}

pair<Individual, Individual> partially_mapped_crossover(Individual parent1, Individual parent2) {
    //If parents aren't the same size something went very wrong
    if (parent1.size() != parent2.size()) {
        cout << "Parents are different sizes. Exiting..." << endl;
        exit(1);
    }

    //Choose cut points
    int len = parent1.size();
    int pos1 = rand() % (len / 2); //Choose point in left side of array
    int pos2 = len / 2 + (rand() % (len / 2));

    Individual child1(len);
    Individual child2(len);

    //Copy middle section of parents
    for (int i = pos1; i <= pos2; i++) {
        child1[i] = parent1[i];
        child2[i] = parent2[i];
    }

    //Create maps for reallocation of duplicate values
    unordered_map<int, Gene> ch1_map;
    unordered_map<int, Gene> ch2_map;

    for (int i = pos1; i <= pos2; i++) {
        ch1_map[parent1[i].job->id] = parent2[i];
        ch2_map[parent2[i].job->id] = parent1[i];
    }

    //Build new children
    for (int i = 0; i < pos1; i++) {
        Gene ch1_val = parent2[i];
        Gene ch2_val = parent1[i];

        while (ch1_map.find(ch1_val.job->id) != ch1_map.end()) {
            ch1_val = ch1_map[ch1_val.job->id];
        }

        while (ch2_map.find(ch2_val.job->id) != ch2_map.end()) {
            ch2_val = ch2_map[ch2_val.job->id];
        }

        child1[i] = ch1_val;
        child2[i] = ch2_val;
    }

    for (int i = pos2 + 1; i < len; i++) {
        Gene ch1_val = parent2[i];
        Gene ch2_val = parent1[i];

        while (ch1_map.find(ch1_val.job->id) != ch1_map.end()) {
            ch1_val = ch1_map[ch1_val.job->id];
        }

        while (ch2_map.find(ch2_val.job->id) != ch2_map.end()) {
            ch2_val = ch2_map[ch2_val.job->id];
        }

        child1[i] = ch1_val;
        child2[i] = ch2_val;
    }

    return make_pair(child1, child2);
}

pair<Individual, Individual> choose_parents(Population &population, vector<long long> &fitnesses, long long total_fitness) {
    int parent1_pos = -1;
    int parent2_pos = -1;

    //Choose first parent
    do {
        int candidate_pos = rand() % population.size();

        //Calculate its probability of being chosen
        double candidate_prob = (double)fitnesses[candidate_pos] / (double)total_fitness;
        double prob = (double)rand() / (double)RAND_MAX;

        if (prob < candidate_prob) {
            parent1_pos = candidate_pos;
        }

    } while (parent1_pos == -1);

    //Choose second parent
    do {
        int candidate_pos = rand() % population.size();

        //Calculate its probability of being chosen
        double candidate_prob = (double)fitnesses[candidate_pos] / (double)total_fitness;
        double prob = (double)rand() / (double)RAND_MAX;

        if (prob < candidate_prob) {
            parent2_pos = candidate_pos;
        }

    } while (parent2_pos == -1);

    return make_pair(population[parent1_pos], population[parent2_pos]);
}

//TO-DO
Fenotype genetic_algorithm(vector<Job*> jobs, int machine_count, int population_size, float mutation_rate, int max_iter) {
    Population population = generate_population(jobs, machine_count, population_size);
    int generation = 0;
    vector<long long> fitnesses(population_size);
    //Sort population by fitness
    sort(population.begin(),
            population.end(),
            [machine_count](const Individual &ind1, const Individual &ind2) {
                return fitness(get_fenotype(ind1, machine_count)) > fitness(get_fenotype(ind2, machine_count));
            }
    );

    do {
        // Compute all fitnesses and sum of all fitnesses
        long long total_fitness_sum = 0;
        for (int i = 0; i < population_size; i++) {
            fitnesses[i] = fitness(get_fenotype(population[i], machine_count));
            total_fitness_sum += fitnesses[i];
        }

        Population new_pop(population_size);
        int new_pop_count = 0;

        //Make population 90% of new children
        while (true) {
            Individual p1, p2;
            tie(p1, p2) = choose_parents(population, fitnesses, total_fitness_sum);

            Individual ch1, ch2;
            tie(ch1, ch2) = partially_mapped_crossover(p1, p2);

            new_pop[new_pop_count++] = ch1;
            if (new_pop_count >= (population_size*9)/10) {
                break;
            }

            new_pop[new_pop_count++] = ch2;
            if (new_pop_count >= (population_size*9)/10) {
                break;
            }
        }

        //Add remaining 10% of parents
        for (int i = new_pop_count; i < population_size; i++) {
            new_pop[i] = population[i - new_pop_count];
        }

        //Mutate the generated population
        for (Individual ind : new_pop) {
            mutate(ind, machine_count, mutation_rate);
        }

        // Replace current population with new one
        population = new_pop;

        //Sort population by fitness
        sort(population.begin(),
             population.end(),
             [machine_count](const Individual &ind1, const Individual &ind2) {
                 return fitness(get_fenotype(ind1, machine_count)) > fitness(get_fenotype(ind2, machine_count));
        });
    } while (fitness(get_fenotype(population[0], machine_count)) != 0 && generation++ < max_iter);

    return get_fenotype(population[0], machine_count);
}