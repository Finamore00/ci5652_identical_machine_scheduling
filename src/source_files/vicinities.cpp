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

using namespace std;

/**
 * Function that takes in a machine's scheduled tasks and calculates the total
 * tardiness of said machine. The total tardiness of a machine is defined as the
 * sum of the tardiness for each Job within the given schedule
*/
long long machine_tardiness(vector<Job> machine_schedule) {
    long long res = 0;
    long long spent_time = 0;
    for (Job j : machine_schedule) {
        spent_time += j.ptime;
        long long tardiness = max(0LL, spent_time - j.dDate);
        res += tardiness;
    }
    return res;
}

/**
 * Neighborhood function consisting of choosing a random Job from the machine
 * with the highest tardiness and relocating it to a random position in the
 * same machine.
*/
vector<vector<Job>> shift_neighborhood(vector<vector<Job>> schedule, int tardiest_machine) {
    srand((unsigned) time(0));

    vector<Job> &machine = schedule[tardiest_machine];

    //If machine is empty don't do anything
    if (machine.empty()) {
        return schedule;
    }

    //Select two random jobs from the machine
    int first_pos = rand() % machine.size();
    int second_pos = rand() % machine.size();

    //Make sure second_pos is different from first_pos
    while (second_pos == first_pos && machine.size() > 1) {
        second_pos = rand() % machine.size();
    }

    //Remove the job from the machine
    Job selected_job = machine[first_pos];
    machine.erase(machine.begin() + first_pos);

    //Reinsert it
    machine.insert(machine.begin() + second_pos, selected_job);

    return schedule;
}

/**
 * Neighborhood function that consists of choosing two jobs randomly within
 * the tardiest machine and swapping their positions within the machine
 * schedule.
*/
vector<vector<Job>> switch_neighborhood(vector<vector<Job>> schedule, int tardiest_machine) {
    srand((unsigned) time(0));

    vector<Job> &machine = schedule[tardiest_machine];
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
vector<vector<Job>> direct_swap_neighborhood(vector<vector<Job>> schedule, int tardiest_machine) {
    srand((unsigned) time(0));

    vector<Job> &target_machine = schedule[tardiest_machine];
    vector<Job> &aux_machine = schedule[rand() % schedule.size()];

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
vector<vector<Job>> two_shift_neighborhood(vector<vector<Job>> schedule, int tardiest_machine) {
    return shift_neighborhood(shift_neighborhood(schedule, tardiest_machine), tardiest_machine);
}

/**
 * Neighborhood function consisting of randomly choosing a machine from the schedule 
 * different from the tardiest machine and extracting a randomly chosen job from each one.
 * The extracted jobs are then inserted in a random position in the opposite machine.
*/
vector<vector<Job>> task_move_neighborhood(vector<vector<Job>> schedule, int tardiest_machine) {
    srand((unsigned) time(0));

    vector<Job> &target_machine = schedule[tardiest_machine];
    vector<Job> &aux_machine = schedule[rand() % schedule.size()];

    //If either machine is empty do nothing.
    if (target_machine.empty() || aux_machine.empty()) {
        return schedule;
    }

    //Choose random position for each machine
    int target_machine_pos = rand() % target_machine.size();
    int aux_machine_pos = rand() % aux_machine.size();

    //Extract the chosen jobs
    Job target_machine_job = target_machine[target_machine_pos];
    Job aux_machine_job = aux_machine[aux_machine_pos];

    //Delete them from each machine
    target_machine.erase(target_machine.begin() + target_machine_pos);
    aux_machine.erase(aux_machine.begin() + aux_machine_pos);

    //Reinsert the jobs in random position of the opposite machine
    target_machine.insert(target_machine.begin() + (rand() % target_machine.size()), aux_machine_job);
    aux_machine.insert(aux_machine.begin() + (rand() % aux_machine.size()), target_machine_job);

    return schedule;
}

/**
 * Function that generates a neighbor for the inputted solution state using
 * a random vicinity function. 
*/
vector<vector<Job>> generate_neighbor(vector<vector<Job>> schedule, int tardiest_machine) {
    srand((unsigned)time(0));

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