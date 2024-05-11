#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Job {
    int id;
    int ptime;
    int dDate;
};

bool compare_jobs(Job a, Job b) {
    return a.dDate - a.ptime < b.dDate - b.ptime;
}

int calculateTardiness(vector<Job>& jobs, vector<vector<Job>>& schedules, int numMachines) {
    int totalTardiness = 0;
    vector<int> machineCompletionTimes(numMachines, 0);
    
    for (int i = 0; i < numMachines; i++) {
        int completionTime = 0;
        for (Job job : schedules[i]) {
            completionTime += job.ptime;
            int tardiness = max(0, completionTime - job.dDate);
            totalTardiness += tardiness;
        }
    }
    
    return totalTardiness;
}

// jobs: list of the unscheduled jobs U = {1, 2, ..., n} 
// numMachines: number of machines m
vector<vector<Job>> mddScheduling(vector<Job> jobs, int numMachines) {
    // list of schedules for each machine
    vector<vector<Job>> schedules(numMachines);

    // Let C_j for j = 1, ..., m be the sum of processing times of the jobs that have already been scheduled on machine j
    vector<int> C(numMachines, 0);

    // while U is not empty
    while (!jobs.empty()) {

        // Divide U into Uj1 and Uj2 such that 
        // Uj1 = {i in U | Cj + pi > di} (the set of jobs that cannot be completed by their due date on machine j)
        // Uj2 = {i in U | Cj + pi <= di} = U - Uj1 (the set of jobs that can be completed by their due date on machine j)
        vector<vector<Job>> U1(numMachines);
        vector<vector<Job>> U2(numMachines);

        for (int j = 0; j < numMachines; j++) {
            for (Job job : jobs) {
                if (C[j] + job.ptime > job.dDate) {
                    U1[j].push_back(job);
                } else {
                    U2[j].push_back(job);
                }
            }
        }

        // print U1 and U2 for each machine
        for (int j = 0; j < numMachines; j++) {
            cout << "U1[" << j << "]: ";
            for (Job job : U1[j]) {
                cout << job.id << " ";
            }
            cout << "\n";

            cout << "U2[" << j << "]: ";
            for (Job job : U2[j]) {
                cout << job.id << " ";
            }
            cout << "\n";
        }

        // For each machine j, find two sets s1j and s2j of jobs such that
        // s1j = {i in Uj1 | pi = min{p in Uj1}} (the set of jobs in Uj1 with the smallest processing time)
        // s2j = {i in Uj2 | di = min{d in Uj2}} (the set of jobs in Uj2 with the smallest due date)

        vector<vector<Job>> s1j(numMachines);
        vector<vector<Job>> s2j(numMachines);

        for (int j = 0; j < numMachines; j++) {
            if (!U1[j].empty()) {
                // Find the minimum processing time in U1[j]
                int minptime = INT_MAX;
                for (Job job : U1[j]) {
                    if (job.ptime < minptime) {
                        minptime = job.ptime;
                    }
                }

                // Add jobs with minimum processing time to s1j[j]
                for (Job job : U1[j]) {
                    if (job.ptime == minptime) {
                        s1j[j].push_back(job);
                    }
                }
            }

            if (!U2[j].empty()) {
                // Find the minimum due date in U2[j]
                int mindDate = INT_MAX;
                for (Job job : U2[j]) {
                    if (job.dDate < mindDate) {
                        mindDate = job.dDate;
                    }
                }

                // Add jobs with minimum due date to s2j[j]
                for (Job job : U2[j]) {
                    if (job.dDate == mindDate) {
                        s2j[j].push_back(job);
                    }
                }
            }
        }

        // print s1j and s2j for each machine
        for (int j = 0; j < numMachines; j++) {
            cout << "s1j[" << j << "]: ";
            for (Job job : s1j[j]) {
                cout << job.id << " ";
            }
            cout << "\n";

            cout << "s2j[" << j << "]: ";
            for (Job job : s2j[j]) {
                cout << job.id << " ";
            }
            cout << "\n";
        }

        break;
        /*

        // Let aj be any job from s1j and bj be any job from s2j
        // choose job gj in {aj, bj} that satisfies the following conditions:
        // delta_gj = min(Cj + p_aj, d_bj) for j = 1, ..., m 

        vector<Job> g(numMachines);
        for (int j = 0; j < numMachines; j++) {
            if (!s1j[j].empty() && !s2j[j].empty()) {
                int deltaAj = C[j] + s1j[j].back().ptime;
                int deltaBj = s2j[j].back().dDate;
                g[j] = deltaAj < deltaBj ? s1j[j].back() : s2j[j].back();
            } else if (!s1j[j].empty()) {
                g[j] = s1j[j].back();
            } else {
                g[j] = s2j[j].back();
            }
        }

        // Schedule job gl on machine l such that
        // delta_gl = min{delta_gj} for j = 1, ..., m
        // and put job gl in the last position of machile l.
        // Remove job gl from setU

        int minDelta = INT_MAX;
        for (int j = 0; j < numMachines; j++) {
            if (g[j].ptime + C[j] < g[j].dDate) {
                if (g[j].ptime + C[j] < minDelta) {
                    minDelta = g[j].ptime + C[j];
                }
            } else {
                if (g[j].dDate < minDelta) {
                    minDelta = g[j].dDate;
                }
            }
        }

        for (int j = 0; j < numMachines; j++) {
            if (g[j].ptime + C[j] == minDelta || g[j].dDate == minDelta) {
                schedules[j].push_back(g[j]);
                C[j] += g[j].ptime;
                jobs.erase(find(jobs.begin(), jobs.end(), g[j]));
            }
        }
        */
    }

    return schedules;
}

int main() {
    int numJobs, numMachines;
    cout << "Enter the number of jobs: ";
    cin >> numJobs;
    cout << "Enter the number of machines: ";
    cin >> numMachines;
    
    vector<Job> jobs(numJobs);
    for (int i = 0; i < numJobs; i++) {
        cout << "Enter processing time and due date for job " << i + 1 << ": ";
        cin >> jobs[i].ptime >> jobs[i].dDate;
        jobs[i].id = i + 1;
    }
    
    vector<vector<Job>> schedules = mddScheduling(jobs, numMachines);
    int totalTardiness = calculateTardiness(jobs, schedules, numMachines);
    
    cout << "Schedules:\n";
    for (int i = 0; i < numMachines; i++) {
        cout << "Machine " << i + 1 << ": ";
        for (Job job : schedules[i]) {
            cout << job.id << " ";
        }
        cout << "\n";
    }
    
    cout << "Total tardiness: " << totalTardiness << "\n";
    
    return 0;
}