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
vector<vector<Job*>> shift_neighborhood(vector<vector<Job*>> &schedule, int tardiest_machine) {
    if (schedule[tardiest_machine].size() < 2) {
        return schedule;
    }

    auto now = high_resolution_clock::now();
	auto timeMillis = duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
	srand(timeMillis);

    //Select a random job from the machine
    int first_pos = rand() % schedule[tardiest_machine].size();
    Job* selected_job = schedule[tardiest_machine][first_pos];

    // Choose a random position for the job to be inserted within the tardiest machine
    // Ensuring that the job is not inserted in the same position it was extracted from
    int second_pos = rand() % schedule[tardiest_machine].size();
    while (second_pos == first_pos) {
        second_pos = rand() % schedule[tardiest_machine].size();
    }

    schedule[tardiest_machine].erase(schedule[tardiest_machine].begin() + first_pos);
    
    schedule[tardiest_machine].insert(schedule[tardiest_machine].begin() + second_pos, selected_job);


    return schedule;
}

/**
 * Neighborhood function that consists of choosing two jobs randomly within
 * the tardiest machine and swapping their positions within the machine
 * schedule.
*/
vector<vector<Job*>> switch_neighborhood(vector<vector<Job*>> &schedule, int tardiest_machine) {
    auto now = high_resolution_clock::now();
	auto timeMillis = duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
	srand(timeMillis);

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
vector<vector<Job*>> direct_swap_neighborhood(vector<vector<Job*>> &schedule, int tardiest_machine) {
    auto now = high_resolution_clock::now();
	auto timeMillis = duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
	srand(timeMillis);

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
vector<vector<Job*>> two_shift_neighborhood(vector<vector<Job*>> &schedule, int tardiest_machine) {
    vector<vector<Job*>> first_shift = shift_neighborhood(schedule, tardiest_machine);
    return shift_neighborhood(first_shift, tardiest_machine);
}

/**
 * Neighborhood function consisting of randomly choosing a machine from the schedule 
 * different from the tardiest machine and extracting a randomly chosen job from each one.
 * The extracted jobs are then inserted in a random position in the opposite machine.
*/
vector<vector<Job*>> task_move_neighborhood(vector<vector<Job*>> &schedule, int tardiest_machine) {
    auto now = high_resolution_clock::now();
	auto timeMillis = duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
	srand(timeMillis);

    vector<Job*> &target_machine = schedule[tardiest_machine];
    vector<Job*> &aux_machine = schedule[rand() % schedule.size()];

    //If either machine is empty do nothing.
    if (target_machine.empty() || aux_machine.empty()) {
        return schedule;
    }

    //Choose random position for each machine
    int target_machine_pos = rand() % target_machine.size();
    int aux_machine_pos = rand() % aux_machine.size();

    //Extract the chosen jobs
    Job* target_machine_job = target_machine[target_machine_pos];
    Job* aux_machine_job = aux_machine[aux_machine_pos];

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
vector<vector<Job*>> generate_neighbor(vector<vector<Job*>> &schedule, int tardiest_machine) {
    srand((unsigned)time(0));

    int chosen_function = 0;

    switch (chosen_function) {
        case 0:
            cout << "Shift neighborhood" << endl;
            return shift_neighborhood(schedule, tardiest_machine);
        case 1:
            cout << "Switch neighborhood" << endl;
            return switch_neighborhood(schedule, tardiest_machine);
        case 2:
            cout << "Direct swap neighborhood" << endl;
            return direct_swap_neighborhood(schedule, tardiest_machine);
        case 3:
            cout << "Two shift neighborhood" << endl;
            return two_shift_neighborhood(schedule, tardiest_machine);
        case 4:
            cout << "Task move neighborhood" << endl;
            return task_move_neighborhood(schedule, tardiest_machine);
        default:
            exit(1); //If switch statement doesn't work as intended then something is terribly wrong.
    }
}