/**
 * Neighborhood functions for the local search algorithm. All vectors are passed
 * by value as modifying the current target solution would affect the generation of
 * following vicinity elements.
**/

#include "../header_files/Job.h"
#include "../header_files/heuristic.h"
#include <vector>
#include <random>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace chrono;

/**
 * Function that takes in a machine's scheduled tasks and calculates the total
 * tardiness of said machine. The total tardiness of a machine is defined as the
 * sum of the tardiness for each Job within the given schedule
*/
long long machine_tardiness(vector<Job*> machine_schedule) {
    long long res = 0;
    long long spent_time = 0;
    for (Job* j : machine_schedule) {
        spent_time += j->ptime;
        long long tardiness = max(0LL, spent_time - j->dDate);
        res += tardiness;
    }
    return res;
}

void imprimir(vector<vector<Job*>> &schedule, int machine_count) {
    for (int i = 0; i < machine_count; i++) {
        cout << "\nMachine " << i + 1 << ": ";
        for (Job* job : schedule[i]) {
            cout << job->id << " ";
        }
        cout << endl;
    }
}

/**
 * Neighborhood function consisting of choosing a random Job from the machine
 * with the highest tardiness and relocating it to a random position in the
 * same machine.
*/
vector<vector<Job*>> shift_neighborhood(vector<vector<Job*>> schedule, int tardiest_machine) {

    if (schedule[tardiest_machine].size() < 2) {
        return schedule;
    }

    //Select a random job from the machine
    int first_pos = rand() % schedule[tardiest_machine].size();
    Job* selected_job = schedule[tardiest_machine][first_pos];

    schedule[tardiest_machine].erase(schedule[tardiest_machine].begin() + first_pos);

    // Choose a random position for the job to be inserted within the tardiest machine
    int second_pos = rand() % schedule[tardiest_machine].size();
    schedule[tardiest_machine].insert(schedule[tardiest_machine].begin() + second_pos, selected_job);

    return schedule;
}

/**
 * Neighborhood function that consists of choosing two jobs randomly within
 * the tardiest machine and swapping their positions within the machine
 * schedule.
*/
vector<vector<Job*>> switch_neighborhood(vector<vector<Job*>> schedule, int tardiest_machine) {

    vector<Job*> &machine = schedule[tardiest_machine];
    //If machine is empty don't do anything
    if (machine.empty()) {
        return schedule;
    }

    //Choose two random positions within the machine
    int first_pos = rand() % machine.size();
    int second_pos = rand() % machine.size();

    //Swap their values
    std::swap(machine[first_pos], machine[second_pos]);

    return schedule;
}

/**
 * Neighborhood function that consists of choosing a random machine different
 * from the tardiest machine, and then exchanging two random jobs between the
 * two, respecting the position each job had within their respective machine
*/
vector<vector<Job*>> direct_swap_neighborhood(vector<vector<Job*>> schedule, int tardiest_machine) {

    vector<Job*> &target_machine = schedule[tardiest_machine];
    vector<Job*> &aux_machine = schedule[rand() % schedule.size()];

    //If either machine is empty do nothing.
    if (target_machine.empty() || aux_machine.empty()) {
        return schedule;
    }

    //Select one position in each machine
    int target_machine_pos = rand() % target_machine.size();
    int aux_machine_pos = rand() % aux_machine.size();

    //Swap both job values
    std::swap(target_machine[target_machine_pos], aux_machine[aux_machine_pos]);

    return schedule;
}

/**
 * Neighborhood function that consists of switching the position of two jobs within 
 * a given machine. It's basically equivalent to a double application of the shift
 * neighborhood function
*/
vector<vector<Job*>> two_shift_neighborhood(vector<vector<Job*>> schedule, int tardiest_machine) {
    vector<vector<Job*>> first_shift = shift_neighborhood(schedule, tardiest_machine);
    return shift_neighborhood(first_shift, tardiest_machine);
}

/**
 * Neighborhood function consisting of randomly choosing a machine from the schedule 
 * different from the tardiest machine and extracting a randomly chosen job from each one.
 * The extracted jobs are then inserted in a random position in the opposite machine.
*/
vector<vector<Job*>> task_move_neighborhood(vector<vector<Job*>> schedule, int tardiest_machine) {

    vector<Job*> &target_machine = schedule[tardiest_machine];
    int aux_machine_idx = rand() % schedule.size();
    vector<Job *> &aux_machine = schedule[rand() % schedule.size()];

    //If either machine is empty do nothing.
    if (target_machine.empty() || aux_machine.empty() || aux_machine == target_machine) {
        return schedule;
    }

    //Choose a job for each machine, store it and delete it from vector
    int target_machine_pos = rand() % target_machine.size();
    Job *target_machine_job = target_machine[target_machine_pos];
    target_machine.erase(target_machine.begin() + target_machine_pos);

    int aux_machine_pos = aux_machine.empty() ? 0 : rand() % aux_machine.size();
    Job* aux_machine_job = aux_machine[aux_machine_pos];
    aux_machine.erase(aux_machine.begin() + aux_machine_pos);

    //Get new insertion possitions
    int target_machine_insert_pos = target_machine.empty() ? 0 : rand() % target_machine.size();
    int aux_machine_insert_pos = aux_machine.empty() ? 0 : rand() % aux_machine.size();

    //Reinsert the jobs in random position of the opposite machine
    target_machine.insert(target_machine.begin() + target_machine_insert_pos, aux_machine_job);
    aux_machine.insert(aux_machine.begin() + aux_machine_insert_pos, target_machine_job);

    return schedule;
}

/**
 * Function that generates a neighbor for the inputted solution state using
 * a random vicinity function. 
*/
vector<vector<Job*>> generate_neighbor(vector<vector<Job*>> schedule, int tardiest_machine) {

    int chosen_function = rand() % 5;

    switch (chosen_function) {
        case 0:
            return shift_neighborhood(schedule, tardiest_machine);
        case 1:
            return switch_neighborhood(schedule, tardiest_machine);
        case 2:
            return direct_swap_neighborhood(schedule, tardiest_machine);
        case 3:
            return two_shift_neighborhood(schedule, tardiest_machine);
        case 4:
            return task_move_neighborhood(schedule, tardiest_machine);
        default:
            exit(1); //If switch statement doesn't work as intended then something is terribly wrong.
    }
}

int index_tardiest_machine(vector<vector<Job*>> schedule) {
    int tardiest_machine = -1;
    long long max_tardiness = LLONG_MIN;
    for (size_t j = 0; j < schedule.size(); j++) {
        if (machine_tardiness(schedule[j]) > max_tardiness) {
                max_tardiness = machine_tardiness(schedule[j]);
                tardiest_machine = j;
        }
    }
    return tardiest_machine;
}