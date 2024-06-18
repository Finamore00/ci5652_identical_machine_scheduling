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

    Fenotype memetic_algorithm(vector<Job *> jobs, int machine_count, int population_size, float mutation_rate, int max_iter, int nro_parents_crossover);
#endif