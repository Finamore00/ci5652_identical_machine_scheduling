#ifndef MEMETIC_H
    #define MEMETIC_H
    
    #include <vector>
    #include <unordered_map>
    #include <random>
    #include "Job.h"
    #include "evolution.h"

    using namespace std;
    
    typedef vector<vector<Gene>> Population;
    typedef vector<Gene> Individual;
    typedef vector<vector<Job*>> Fenotype;

    Individual get_genotype(Fenotype fen, int machine_count);
    unordered_map<int, Gene> create_mapping_list(vector<Individual> parents);
    vector<vector<int>> create_random_order(int n, int nro_genes, vector<int> cut_points);
    vector<Individual> multi_partially_mapped_crossover(vector<Individual> parents);
    vector<Individual> choose_multi_parents(int k, Population &population, vector<long long> &fitnesses, long long total_fitness);
    Fenotype memetic_algorithm(vector<Job*> jobs, int machine_count, int population_size, float mutation_rate, int max_iter, int nro_parents_crossover, int opt_freq, float opt_rate, bool random_opt_rate);
#endif