#ifndef SCATTERED_H
#define SCATTERED_H
    #include <algorithm>
    #include <vector>
    #include "../header_files/evolution.h"
    #include "../header_files/memetic.h"

    Fenotype scatter_search(vector<Job*> jobs,
                            int machine_count,
                            int population_size,
                            int reference_set_size,
                            int parent_count,
                            int max_iter,
                            double path_relinking_ptg);

    vector<Individual> path_relinking(Individual ind, Individual target);

#endif  // SCATTERED_H
