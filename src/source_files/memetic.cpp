#include <algorithm>
#include <vector>
#include <unordered_map>
#include <random>
#include "../header_files/Job.h"
#include "../header_files/vicinities.h"
#include "../header_files/local_search.h"
#include "../header_files/exact.h"
#include "../header_files/memetic.h"
#include "../header_files/evolution.h"


using namespace std;

// mapping list determination for multi partially mapped crossover with n parents
unordered_map<int, Gene> create_mapping_list(vector<Individual> parents) {
     unordered_map<int, Gene> ch_map;

    // n = number of parents
    // m = number of jobs or genes
    int n = parents.size(), m = parents[0].size();
    // array to check if the job is mapped or not
    vector<bool> job_mapped(m, false);

    // random choose start_parent
    int ptr_start_parent = rand() % n;
    Individual start_parent = parents[ptr_start_parent];
    // random chooese a gene from start_parent
    Gene start_gene = start_parent[rand() % m];

    int ptr_end_parent;
    Individual end_parent;
    Gene end_gene;

    Gene initial_gene_mapp = start_gene;

    int cnt_mapped = 0;
    for (int j = 0; j < m; j++) {
        // mark the job as mapped
        job_mapped[start_gene.job->id] = true;

        // select a random parent that is not the same as start_parent
        do {
            ptr_end_parent = rand() % n;
            end_parent = parents[ptr_end_parent];
        } while (ptr_end_parent == ptr_start_parent);

        // in the end_parent, find the gene that has the same job as start_gene
        int pos_end_gene;
        for (int k = 0; k < m; k++) {
            if (end_parent[k].job->id == start_gene.job->id) {
                end_gene = end_parent[k];
                pos_end_gene = k;
                break;
            }
        }

        // now, for the new start_parent,
        // select a random parent that is not the same as end_parent 
        // and select a random gene from the new start_parent that is not mapped
        cnt_mapped++;
        if (cnt_mapped == m) {
            break;
        }
        while (true) {
            ptr_start_parent = rand() % n;
            if (ptr_start_parent == ptr_end_parent) {
                continue;
            }
            start_parent = parents[ptr_start_parent];
            start_gene = start_parent[rand() % m];
            if (!job_mapped[start_gene.job->id]) {
                break;
            }
        }

        // add the mapping to the mapping list
        ch_map[end_gene.job->id] = start_gene;
    }

    // add the mapping of the last gene with the initial gene
    ch_map[end_gene.job->id] = initial_gene_mapp;

    return ch_map;
}

// create a random order array for mapping the genes of each subpart
vector<vector<int>> create_random_order(int n, int nro_genes, vector<int> cut_points) {
    vector<vector<int>> order(n);
    int start_cut_point = cut_points[0], end_cut_point;

    // iterate each subpart except the first one
    for (int i = 1; i < n; i++) {
        end_cut_point = cut_points[i];
        int size_subpart = end_cut_point - start_cut_point;
        vector<int> order_subpart_i(size_subpart);

        // shuffle the order of the subpart
        for (int j = 0; j < size_subpart; j++) {
            order_subpart_i[j] = j;
        }
        random_shuffle(order_subpart_i.begin(), order_subpart_i.end());
        order[i] = order_subpart_i;
        start_cut_point = end_cut_point;
    }

    // create the order array for the genes of the first subpart
    int size_first_subpart = (cut_points[0] + 1) + (nro_genes - start_cut_point - 1);
    vector<int> order_first_subpart(size_first_subpart);
    for (int i = 0; i < size_first_subpart; i++) {
        order_first_subpart[i] = i;
    }
    random_shuffle(order_first_subpart.begin(), order_first_subpart.end());
    order[0] = order_first_subpart;

    return order;
}

// partially mapped crossover with n parents
vector<Individual> multi_partially_mapped_crossover(vector<Individual> parents) {
    // number of parents
    int n = parents.size();

    // randomly selects n crossover points to cut each parent into n + 1 subparts
    vector<bool> already_exists_cut_points(n, false);
    vector<int> cut_points(n);
    for (int i = 0; i < n; i++) {
        cut_points[i] = rand() % parents[i].size();
        while (already_exists_cut_points[cut_points[i]] || cut_points[i] == 0 || cut_points[i] == parents[i].size() - 1){
            cut_points[i] = rand() % parents[i].size();
        }
        already_exists_cut_points[cut_points[i]] = true;
    }
    sort(cut_points.begin(), cut_points.end());
 
    // mapping list determination by id jobs
    unordered_map<int, Gene> ch_map = create_mapping_list(parents);

    // create an order array for mapping each gene
    vector<vector<int>> order = create_random_order(n, parents[0].size(), cut_points);

    // create the n children
    vector<Individual> children(n);

    for (int i = 0; i < n; i++) {
        // create the child i 
        Individual child_i(parents[i].size());

        // copy the first subpart from the parent i of both sides (the start and the end sides)
        for (int j = 0; j < cut_points[0]; j++) {
            child_i[j] = parents[i][j];
        }

        for (int j = cut_points[cut_points.size() - 1]; j < parents[i].size(); j++) {
            child_i[j] = parents[i][j];
        }

        // copy the other subparts from other parents in diagonal
        int start_parent_copy = (i + 1) % n;
        for (int j = 0; j < n - 1; j++) {
            int start_subpart = cut_points[j];
            int end_subpart = cut_points[j+1] - 1;
            for (int k = start_subpart; k <= end_subpart; k++) {
                child_i[k] = parents[start_parent_copy][k];
            }
            start_parent_copy = (start_parent_copy + 1) % n;
        }

        children[i] = child_i;        
    }


    vector<vector<bool>> marked(n, vector<bool>(parents[0].size() + 1, false));

    // legalize the children
    for (int i = 0; i < n; i++) {
        Individual child = children[i];
        int jobs_marked_i = 0;

        // create an array for the order of each subpart
        vector<int> new_order(n);
        for (int i = 0; i < n; i++) {
            new_order[i] = i;
        }
        random_shuffle(new_order.begin(), new_order.end());

        // if the new_order[0] == 0, then change it to another value
        if (new_order[0] == 0) {
            int temp = new_order[0];
            new_order[0] = new_order[1];
            new_order[1] = temp;
        }

        // get the subpart 0 and mark the jobs
        int pos_subpart = new_order[0];
        for (int k = cut_points[pos_subpart - 1]; k < cut_points[pos_subpart]; k++) {
            marked[i][child[k].job->id] = true;
            jobs_marked_i++;
        }

        // iterate each other subparts according to the new_order
        for (int j = 1; j < new_order.size(); j++) {
            // get the number of subpart j
            pos_subpart = new_order[j];
            int s, e, pos = 0;
            if (pos_subpart == 0) {
                s = 0;
                e = order[pos_subpart].size();
                for (int k = s; k < e; k++) {
                    int order_pos_gene = order[pos_subpart][pos];
                    if (order_pos_gene > cut_points[0] - 1) {
                        order_pos_gene = cut_points[n-1] + (order_pos_gene - cut_points[0]);
                    } else {
                        order_pos_gene += s;
                    }
                    Gene gene_curr = child[order_pos_gene];
                    int job_curr = gene_curr.job->id;
                    while (marked[i][job_curr]) {
                        gene_curr = ch_map[job_curr];
                        job_curr = gene_curr.job->id;
                    }
                    marked[i][job_curr] = true;
                    child[order_pos_gene] = gene_curr;
                    pos++;
                    jobs_marked_i++;
                }
            } else {
                s = cut_points[pos_subpart - 1];
                e = cut_points[pos_subpart];
                for (int k = s; k < e; k++) {
                    int order_pos_gene = order[pos_subpart][pos];
                    order_pos_gene += s;
                    Gene gene_curr = child[order_pos_gene];
                    int job_curr = gene_curr.job->id;
                    while (marked[i][job_curr]) {
                        gene_curr = ch_map[job_curr];
                        job_curr = gene_curr.job->id;
                    }
                    marked[i][job_curr] = true;
                    child[order_pos_gene] = gene_curr;
                    pos++;
                    jobs_marked_i++;
                }
            }
        }

        children[i] = child;

        // check if all jobs are marked, if not then there is an error
        if (jobs_marked_i != child.size()) {
            cout << "ERROR: jobs_marked_i != child.size()" << endl;
            exit(1);
        }

    }

    return children;
}

vector<Individual> choose_multi_parents(int k, Population &population, vector<long long> &fitnesses, long long total_fitness) {
    // Choose k parents
    vector<Individual> parents;
    for (int i = 0; i < k; i++) {
        int parent_pos = -1;

        //Choose parent
        do {
            int candidate_pos = rand() % population.size();

            //Calculate its probability of being chosen
            double candidate_prob = (double)fitnesses[candidate_pos] / (double)total_fitness;
            double prob = (double)rand() / (double)RAND_MAX;

            if (prob < candidate_prob) {
                parent_pos = candidate_pos;
            }

        } while (parent_pos == -1);

        parents.push_back(population[parent_pos]);
    }

    return parents;
}


//TO-DO
/**
 * Applies a genetic algorithm to solve the job scheduling problem.
 *
 * @param jobs The list of jobs to be scheduled.
 * @param machine_count The number of machines available for scheduling.
 * @param population_size The size of the population for the genetic algorithm.
 * @param mutation_rate The rate at which mutations occur during the genetic algorithm.
 * @param max_iter The maximum number of iterations for the genetic algorithm.
 * @return The best fenotype (job scheduling) solution found by the genetic algorithm.
 */
Fenotype memetic_algorithm(vector<Job*> jobs, int machine_count, int population_size, float mutation_rate, int max_iter, int nro_parents_crossover) {
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

        Population new_pop;
        int new_pop_count = 0;

        //Make population 90% of new children
        while (true) {
            vector<Individual> selected_parents = choose_multi_parents(nro_parents_crossover, population, fitnesses, total_fitness_sum);

            vector<Individual> children = multi_partially_mapped_crossover(selected_parents);

            for (Individual child : children) {
                new_pop.push_back(child);
            }

            if (new_pop.size() >= (population_size * 0.9)) {
                break;
            }

        }

        new_pop_count = new_pop.size();

        //Add remaining 10% of parents
        for (int i = new_pop_count; i < population_size; i++) {
            new_pop.push_back(population[i - new_pop_count]);
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
