#ifndef TINDER_SEARCH_H
    #define TINDER_SEARCH_H
    
    #include <vector>
    #include <random>
    #include "Job.h"
    #include "evolution.h"

    using namespace std;
    
    typedef vector<vector<Gene>> Population;
    typedef vector<Gene> Individual;
    typedef vector<vector<Job*>> Fenotype;

    pair<int, int> count_likes_dislikes(vector<vector<int>> preferences);
    vector<vector<int>> generate_random_preferences(int n, int m);
    float calculate_atraction(vector<vector<int>> individualPreferences, Individual individualB);
    void update_likes(vector<vector<int>> &LikerPreferences, Individual couple);
    void update_dislikes(vector<vector<int>> &GhosterPreferences, Individual ghoster);
    pair<int, int> choose_couple(Population &population, vector<vector<vector<int>>> &preferences, int nro_profiles, float delta, bool to_ghost, int nro_msgs) ;
    vector<vector<int>> mix_preferences(vector<vector<int>> preferences1, vector<vector<int>> preferences2);
    Fenotype tinder_search(vector<Job *> jobs, int machine_count, int population_size, float mutation_rate, int max_iter, int nro_profiles, float delta, bool to_ghost, int nro_msgs);
#endif