#ifndef SCATTERED_H
#define SCATTERED_H
    #include <algorithm>
    #include <vector>
    #include <climits>
    #include "../header_files/evolution.h"
    #include "../header_files/memetic.h"
    #include "../header_files/local_search.h"

    int find_job_in_individual(Individual ind, int job_id);
    bool equal_individuals(Individual ind1, Individual ind2);
    Individual path_relinking(Individual ind, Individual target, int machine_count);
    int distance_function(Individual ind1, Individual ind2);
    int minimum_distance_to_ref_set(Population ref_set, Individual pool_sample);
    Population generate_reference_set(Population pool, int ref_set_size, int machine_count);
    Fenotype scatter_search(vector<Job *> jobs, int machine_count, int max_pool_size, int ref_set_size, int parent_count, int max_iter, double path_relinking_ptg);

#endif  // SCATTERED_H
