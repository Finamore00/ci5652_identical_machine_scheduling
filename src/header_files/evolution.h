#ifndef EVOLUTION_H
    #define EVOLUTION_H

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

    typedef vector<vector<Gene>> Population;
    typedef vector<Gene> Individual;
    typedef vector<vector<Job*>> Fenotype;

    void print_genotype(Individual ind);
    Individual generate_individual(vector<Job *> jobs, int machine_count);
    Population generate_population(vector<Job *> jobs, int machine_count, int population_size);
    Fenotype get_fenotype(Individual individual, int machine_count);
    long long fitness(Fenotype fen);
    void mutate(Individual &ind, int machine_count, float mutation_rate);
    pair<Individual, Individual> partially_mapped_crossover(Individual parent1, Individual parent2);
    pair<Individual, Individual> choose_parents(Population &population, vector<long long> &fitnesses, long long total_fitness);
    Fenotype genetic_algorithm(vector<Job *> jobs, int machine_count, int population_size, float mutation_rate, int max_iter);

#endif