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
    
    vector<Job> machine = schedule[tardiest_machine];

    int remove_pos = rand() % machine.size();
    Job chosen_job = machine[remove_pos];
    machine.erase(machine.begin() + remove_pos);

    int insert_pos = rand() % machine.size();
    while (insert_pos == remove_pos) {
        insert_pos = rand() % machine.size();
    }

    machine.insert(machine.begin() + insert_pos, chosen_job);
    return schedule;
}

/**
 * Neighborhood function that consists of choosing two jobs randomly within
 * the tardiest machine and swapping their positions within the machine
 * schedule.
*/
vector<vector<Job>> switch_neighborhood(vector<vector<Job>> schedule, int tardiest_machine) {
    srand((unsigned) time(0));

    vector<Job> machine = schedule[tardiest_machine];

    int pos_1 = rand() % machine.size();
    int pos_2 = rand() % machine.size();

    iter_swap(machine.begin() + pos_1, machine.begin() + pos_2);

    return schedule;
}

/**
 * Neighborhood function that consists of choosing a random machine different
 * from the tardiest machine, and then exchanging two random jobs between the
 * two, respecting the position each job had within their respective machine
*/
vector<vector<Job>> direct_swap_neighborhood(vector<vector<Job>> schedule, int tardiest_machine) {
    srand((unsigned) time(0));

    vector<Job> &machine = schedule[tardiest_machine];
    
    int second_machine_pos = rand() % schedule.size();
    while (second_machine_pos == tardiest_machine) {
        second_machine_pos = rand() % schedule.size();
    }

    vector<Job> &second_machine = schedule[second_machine_pos];
    iter_swap(machine.begin() + rand() % machine.size(), second_machine.begin() + rand() % second_machine.size());

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

    vector<Job> &first_machine = schedule[tardiest_machine];
    vector<Job> &second_machine = schedule[rand() % schedule.size()];

    int first_machine_job_pos = rand() % first_machine.size();
    int second_machine_job_pos = rand() % second_machine.size();

    Job first_job = first_machine[first_machine_job_pos];
    Job second_job = second_machine[second_machine_job_pos];

    first_machine.erase(first_machine.begin() + first_machine_job_pos);
    second_machine.erase(second_machine.begin() + second_machine_job_pos);

    first_machine.insert(first_machine.begin() + (rand() % first_machine.size()), second_job);
    second_machine.insert(second_machine.begin() + (rand() % second_machine.size()), first_job);

    return schedule;
}