#include "../header_files/Job.h"
#include "../header_files/heuristic.h"
#include "../header_files/vicinities.h"
#include "../header_files/exact.h"

using namespace std;


vector<vector<Job*>> simulated_annealing(vector<Job*> jobs, int machines, float T, int Tstep, int MaxP, int MaxIt, int MaxN, int MaxNe) {
    vector<vector<Job*>> best_schedule = mddScheduling(jobs, machines);

    int it = 0, it_p = 0, nb_p = 0, nb_ne = 0;
    bool test = false;
    int test_id = 0;
    vector<vector<Job*>> neighbor;
    long long new_tardiness, current_tardiness = total_tardiness(best_schedule);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    while (it < MaxIt && nb_p < MaxP) {
        it++;
        it_p++;
        test = false;

        for (size_t i = 0; i < best_schedule.size(); i++) {
            if (best_schedule[i].empty()) {
                test = true;
                test_id = i;
                break;
            }
        }

        if (test) {
            neighbor = best_schedule;
            vector<int> non_empty_machines;
            for (size_t i = 0; i < neighbor.size(); i++) {
                if (!neighbor[i].empty()) {
                    non_empty_machines.push_back(i);
                }
            }

            uniform_int_distribution<> machine_dis(0, non_empty_machines.size() - 1);
            int random_machine_idx = machine_dis(gen);
            int source_machine = non_empty_machines[random_machine_idx];

            uniform_int_distribution<> job_dis(0, neighbor[source_machine].size() - 1);
            int random_job_idx = job_dis(gen);
            Job* job_to_move = neighbor[source_machine][random_job_idx];

            neighbor[source_machine].erase(neighbor[source_machine].begin() + random_job_idx);
            neighbor[test_id].push_back(job_to_move);
        } else {
            uniform_int_distribution<> machine_dis(0, machines - 1);
            int machine1 = machine_dis(gen);
            while (best_schedule[machine1].empty()) {
                machine1 = machine_dis(gen);
            }
            uniform_int_distribution<> job_dis1(0, best_schedule[machine1].size() - 1);
            int job_idx1 = job_dis1(gen);
            Job* rdm1 = best_schedule[machine1][job_idx1];

            int machine2 = machine_dis(gen);
            while (best_schedule[machine2].empty()) {
                machine2 = machine_dis(gen);
            }
            uniform_int_distribution<> job_dis2(0, best_schedule[machine2].size() - 1);
            int job_idx2 = job_dis2(gen);
            Job* rdm2 = best_schedule[machine2][job_idx2];

            neighbor = best_schedule;

            if (it_p % 10 != 0) {
                swap(neighbor[machine1][job_idx1], neighbor[machine2][job_idx2]);
            } else {
                neighbor[machine1].erase(neighbor[machine1].begin() + job_idx1);
                if (job_idx2 + 1 >= neighbor[machine2].size()) {
                    neighbor[machine2].push_back(rdm1);
                } else {
                    neighbor[machine2].insert(neighbor[machine2].begin() + job_idx2 + 1, rdm1);
                }
            }
        }

        new_tardiness = total_tardiness(neighbor);
        if (new_tardiness < current_tardiness) {
            best_schedule = neighbor;
            current_tardiness = new_tardiness;
            nb_ne = 0;
        } else {
            double r = dis(gen);
            long delta = new_tardiness - current_tardiness;
            if (r < exp(-delta / T)) {
                best_schedule = neighbor;
                current_tardiness = new_tardiness;
                nb_ne++;
            }
            if (nb_ne > MaxNe || it_p > MaxN) {
                T *= Tstep;
                nb_ne = 0;
                it_p = 0;
                nb_p++;
            }
        }
    }

    return best_schedule;
}