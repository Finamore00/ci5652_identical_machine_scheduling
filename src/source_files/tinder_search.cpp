#include <algorithm>
#include <vector>
#include <random>
#include "../header_files/Job.h"
#include "../header_files/exact.h"
#include "../header_files/tinder_search.h"
#include "../header_files/evolution.h"

using namespace std;

// count dislikes and likes of an individual
pair<int,int> count_likes_dislikes(vector<vector<int>> preferences) {
    int likes = 0;
    int dislikes = 0;

    for (int i = 0; i < preferences.size(); i++) {
        for (int j = 0; j < preferences[i].size(); j++) {
            if (preferences[i][j] == 1) {
                likes++;
            } else if (preferences[i][j] == -1) {
                dislikes++;
            }
        }
    }

    return make_pair(likes, dislikes);
}


// 1 i like
// -1 i dont like
// 0 neutral
vector<vector<int>> generate_random_preferences(int n, int m) {
    // avoid more than 80% of likes or dislikes
    vector<vector<int>> preferences(n); 
    for (int i = 0; i < n; i++) {
        preferences[i] = vector<int>(m, 0);
    }

    int likes = 0;
    int dislikes = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int random_number = rand() % 3;
            if (random_number == 0 && likes < 0.8 * n * m) {
                preferences[i][j] = 1;
                likes++;
            } else if (random_number == 1 && dislikes < 0.8 * n * m) {
                preferences[i][j] = -1;
                dislikes++;
            } else {
                preferences[i][j] = 0;
            }
        }
    }
    
    return preferences;
}

// calcular nivel de atraccion entre 0 y 1
float calculate_atraction(vector<vector<int>> individualPreferences, Individual individualB) {
    float level_atraction = 0;
    // for each gene of individualB
    for (int i = 0; i < individualB.size(); i++) {
        int job = individualB[i].job->id - 1;
        int machine = individualB[i].machine;

        if (individualPreferences[job][machine] != -1) {
            level_atraction += 1;
        } else {
            level_atraction -= 1;
        }
    }

    if (level_atraction <= 0) {
        return 0;
    }
    return (1 / level_atraction);
}

// update likes of a individual
void update_likes(vector<vector<int>> &LikerPreferences, Individual couple) {
    for (int i = 0; i < couple.size(); i++) {
        int job = couple[i].job->id - 1;
        int machine = couple[i].machine;

        if (LikerPreferences[job][machine] != 1) {
            LikerPreferences[job][machine] = 1;
        }
    }

    // balance preferences
    pair<int,int> likes_dislikes = count_likes_dislikes(LikerPreferences);
    int likes = likes_dislikes.first;

    // avoid more than 80% of likes 
    if (likes > 0.8 * LikerPreferences.size() * LikerPreferences[0].size()) {
        for (int i = 0; i < LikerPreferences.size(); i++) {
            for (int j = 0; j < LikerPreferences[i].size(); j++) {
                // dont update if the gene is from the couple individual
                if (couple[j].job->id == i + 1 && couple[j].machine == j) {
                    continue;
                }

                if (LikerPreferences[i][j] == 1) {
                    int random_number; 
                    if (likes >= 0.8 * LikerPreferences.size() * LikerPreferences[0].size()) {
                        random_number = rand() % 2;
                    } else {
                        random_number = rand() % 3;
                    }
                    
                    if (random_number == 0) {
                        LikerPreferences[i][j] = 0;
                        likes--;
                    } else if (random_number == 1) {
                        LikerPreferences[i][j] = -1;
                        likes--;
                    } else {
                        continue;
                    }
                }
            }   
        }
    }
    return;
}

// update dislikes of a individual
void update_dislikes(vector<vector<int>> &GhosterPreferences, Individual ghosted) {
    for (int i = 0; i < ghosted.size(); i++) {
        int job = ghosted[i].job->id - 1;
        int machine = ghosted[i].machine;

        if (GhosterPreferences[job][machine] != -1) {
            GhosterPreferences[job][machine] = -1;
        }
    }
    
    // balance preferences
    pair<int,int> likes_dislikes = count_likes_dislikes(GhosterPreferences);
    int dislikes = likes_dislikes.second;

    // avoid more than 80% of dislikes
    if (dislikes > 0.8 * GhosterPreferences.size() * GhosterPreferences[0].size()) {
        for (int i = 0; i < GhosterPreferences.size(); i++) {
            for (int j = 0; j < GhosterPreferences[i].size(); j++) {
                // dont update if the gene is from the ghosted individual
                if (ghosted[j].job->id == i + 1 && ghosted[j].machine == j) {
                    continue;
                }

                if (GhosterPreferences[i][j] == 1) {
                    int random_number; 
                    if (dislikes >= 0.8 * GhosterPreferences.size() * GhosterPreferences[0].size()) {
                        random_number = rand() % 2;
                    } else {
                        random_number = rand() % 3;
                    }
                    
                    if (random_number == 0) {
                        GhosterPreferences[i][j] = 0;
                        dislikes--;
                    } else if (random_number == 1) {
                        GhosterPreferences[i][j] = -1;
                        dislikes--;
                    } else {
                        continue;
                    }
                }
            }   
        }
    }
    return;
}

pair<int, int> choose_couple(Population &population, vector<vector<vector<int>>> &preferences, int nro_profiles, float delta, bool to_ghost, int nro_msgs) {
    int parent1_pos = rand() % population.size();
    int parent2_pos = -1;

    float level_desesperation = 0;

    // generate profiles randomly
    vector<int> profiles;
    vector<bool> mark(population.size(), false);

    mark[parent1_pos] = true;
    for (int i = 0; i < nro_profiles; i++) {
        int candidate_pos = rand() % population.size();

        while (mark[candidate_pos]) {
            candidate_pos = rand() % population.size();
        }

        mark[candidate_pos] = true;
        profiles.push_back(candidate_pos);        
    }

    //Choose second parent
    int intento = 0;
    do {
        intento += 1;
        int candidate_pos = rand() % nro_profiles;

        float level_atraction1 = calculate_atraction(preferences[parent1_pos], population[profiles[candidate_pos]]);

        float random_number = (float)rand() / (float)RAND_MAX;

        if (level_atraction1 >= random_number) {
            // swipe left
            level_desesperation += delta;
            continue;
        }

        // swipe right
        float level_atraction2 = calculate_atraction(preferences[profiles[candidate_pos]], population[parent1_pos]); 

        random_number = (float)rand() / (float)RAND_MAX;

        if (level_atraction1 >= random_number) {
            // swipe left
            level_desesperation += delta;
            continue;
        }

        // match
        float compatibility = level_atraction1 * level_atraction2;
        compatibility += level_desesperation;

        if (to_ghost && level_desesperation < 0.5) {
            random_number = (float)rand() / (float)RAND_MAX;

            if (random_number >= 0.5) {
                // ghostea
                level_desesperation += delta;
                // update dislikes
                update_dislikes(preferences[parent1_pos], population[profiles[candidate_pos]]);
                continue;
            }
        }

        // chat
        int count_msgs = 0;
        while (count_msgs < nro_msgs) {
            random_number = (float)rand() / (float)RAND_MAX;

            if (random_number >= compatibility) {
                // end the conversation
                level_desesperation += delta;
                // update dislikes
                update_dislikes(preferences[parent1_pos], population[profiles[candidate_pos]]);
                break;
            } 
            // else continue the conversation
            count_msgs++;
        }

        if (count_msgs == nro_msgs) {
            parent2_pos = profiles[candidate_pos];
        }
    } while (parent2_pos == -1);

    return make_pair(parent1_pos, parent2_pos);
}

vector<vector<int>> mix_preferences(vector<vector<int>> preferences1, vector<vector<int>> preferences2) {
    vector<vector<int>> mixed_preferences(preferences1.size());
    for (int i = 0; i < preferences1.size(); i++) {
        mixed_preferences[i] = vector<int>(preferences1[i].size(), 0);
    }

    for (int i = 0; i < preferences1.size(); i++) {
        for (int j = 0; j < preferences1[i].size(); j++) {
            if (preferences1[i][j] == 1 && preferences2[i][j] == 1) {
                mixed_preferences[i][j] = 1;
            } else if (preferences1[i][j] == -1 && preferences2[i][j] == -1) {
                mixed_preferences[i][j] = -1;
            } else {
                // random number 0, 1 and -1
                int random_number = rand() % 3;
                if (random_number == 0) {
                    mixed_preferences[i][j] = 1;
                } else if (random_number == 1) {
                    mixed_preferences[i][j] = -1;
                } else {
                    mixed_preferences[i][j] = 0;
                }
            }
        }
    }

    return mixed_preferences;
}

Fenotype tinder_search(vector<Job*> jobs, int machine_count, int population_size, float mutation_rate, int max_iter, int nro_profiles, float delta, bool to_ghost, int nro_msgs) {
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

    // generate random preferences for each individual
    vector<vector<vector<int>>> preferences(population_size);
    for (int i = 0; i < population_size; i++) {
        preferences[i] = generate_random_preferences(jobs.size(), machine_count);
    }

    do {

        vector<pair<Individual, vector<vector<int>>>> new_pop;

        //Make population 90% of new children
        while (true) {
            pair<int, int> parents = choose_couple(population, preferences, nro_profiles, delta, to_ghost, nro_msgs);
            int p1 = parents.first;
            int p2 = parents.second;

            // crossover
            pair<Individual, Individual> children = partially_mapped_crossover(population[p1], population[p2]);

            // mix preferences of parents and update likes
            update_likes(preferences[p1], population[p2]);
            update_likes(preferences[p2], population[p1]);

            vector<vector<int>> mixed_preferences1 = mix_preferences(preferences[p1], preferences[p2]);

            // mutate children1
            mutate(children.first, machine_count, mutation_rate);
            new_pop.push_back(make_pair(children.first, mixed_preferences1));

            if (new_pop.size() >= population_size) {
                break;
            }

            // mutate children2
            mutate(children.second, machine_count, mutation_rate);
            vector<vector<int>> mixed_preferences2 = mix_preferences(preferences[p1], preferences[p2]);

            new_pop.push_back(make_pair(children.second, mixed_preferences2));
            if (new_pop.size() >= population_size) {
                break;
            }
        }

        // sort children with its preferences by fitness
        sort(new_pop.begin(),
            new_pop.end(),
            [machine_count](const pair<Individual, vector<vector<int>>> &ind1, const pair<Individual, vector<vector<int>>> &ind2) {
                return fitness(get_fenotype(ind1.first, machine_count)) > fitness(get_fenotype(ind2.first, machine_count));
            }
        );

        // calculate the position of 90% of population
        int new_pop_count = (new_pop.size() * 0.9) + 1;

        //Add remaining 10% of parents who are the best
        for (int i = new_pop_count; i < population_size; i++) {
            new_pop[i] = make_pair(population[i - new_pop_count], preferences[i - new_pop_count]);
        }

        //Update population
        for (int i = 0; i < population_size; i++) {
            population[i] = new_pop[i].first;
            preferences[i] = new_pop[i].second;
        }

        //Sort population by fitness
        sort(population.begin(),
             population.end(),
             [machine_count](const Individual &ind1, const Individual &ind2) {
                 return fitness(get_fenotype(ind1, machine_count)) > fitness(get_fenotype(ind2, machine_count));
        });

    } while (fitness(get_fenotype(population[0], machine_count)) != 0 && generation++ < max_iter);

    return get_fenotype(population[0], machine_count);
}
