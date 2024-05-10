#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Job {
    int id;
    int processingTime;
    int dueDate;
};

bool compare_jobs(Job a, Job b) {
    return a.dueDate - a.processingTime < b.dueDate - b.processingTime;
}

int calculateTardiness(vector<Job>& jobs, vector<vector<Job>>& schedules, int numMachines) {
    int totalTardiness = 0;
    vector<int> machineCompletionTimes(numMachines, 0);
    
    for (int i = 0; i < numMachines; i++) {
        int completionTime = 0;
        for (Job job : schedules[i]) {
            completionTime += job.processingTime;
            int tardiness = max(0, completionTime - job.dueDate);
            totalTardiness += tardiness;
        }
    }
    
    return totalTardiness;
}

vector<vector<Job>> mddScheduling(vector<Job> jobs, int numMachines) {
    // list of the unscheduled jobs U = {1, 2, ..., n}
    // Jobs sorted by due date - processing time in increasing order
    sort(jobs.begin(), jobs.end(), compare_jobs);

     // Schedules for each machine
    vector<vector<Job>> schedules(numMachines);

    // Let C_j for j = 1, ..., m be the sum of processing times of the jobs 
    // that have already been scheduled on machine j
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
                if (C[j] + job.processingTime > job.dueDate) {
                    U1[j].push_back(job);
                } else {
                    U2[j].push_back(job);
                }
            }
        }

        // For each machine j, find two sets s1j and s2j of jobs such that
        // s1j = {i in Uj1 | pi = min{p in Uj1}} (the set of jobs in Uj1 with the smallest processing time)
        // s2j = {i in Uj2 | di = min{d in Uj2}} (the set of jobs in Uj2 with the smallest due date)

        vector<vector<Job>> s1j(numMachines);
        vector<vector<Job>> s2j(numMachines);

        for (int j = 0; j < numMachines; j++) {
            if (!U1[j].empty()) {
                // Find the minimum processing time in U1[j]
                int minProcessingTime = INT_MAX;
                for (Job job : U1[j]) {
                    if (job.processingTime < minProcessingTime) {
                        minProcessingTime = job.processingTime;
                    }
                }

                // Add jobs with minimum processing time to s1j[j]
                for (Job job : U1[j]) {
                    if (job.processingTime == minProcessingTime) {
                        s1j[j].push_back(job);
                    }
                }
            }

            if (!U2[j].empty()) {
                // Find the minimum due date in U2[j]
                int minDueDate = INT_MAX;
                for (Job job : U2[j]) {
                    if (job.dueDate < minDueDate) {
                        minDueDate = job.dueDate;
                    }
                }

                // Add jobs with minimum due date to s2j[j]
                for (Job job : U2[j]) {
                    if (job.dueDate == minDueDate) {
                        s2j[j].push_back(job);
                    }
                }
            }
        }

        // Let aj be any job from s1j and bj be any job from s2j
        // choose job gj in {aj, bj} that satisfies the following conditions:
        // delta_gj = min(Cj + p_aj, d_bj) for j = 1, ..., m 

        vector<Job> g(numMachines);
        for (int j = 0; j < numMachines; j++) {
            if (!s1j[j].empty() && !s2j[j].empty()) {
                int deltaAj = C[j] + s1j[j].back().processingTime;
                int deltaBj = s2j[j].back().dueDate;
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
            if (g[j].processingTime + C[j] < g[j].dueDate) {
                if (g[j].processingTime + C[j] < minDelta) {
                    minDelta = g[j].processingTime + C[j];
                }
            } else {
                if (g[j].dueDate < minDelta) {
                    minDelta = g[j].dueDate;
                }
            }
        }

        for (int j = 0; j < numMachines; j++) {
            if (g[j].processingTime + C[j] == minDelta || g[j].dueDate == minDelta) {
                schedules[j].push_back(g[j]);
                C[j] += g[j].processingTime;
                jobs.erase(find(jobs.begin(), jobs.end(), g[j]));
            }
        }
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
        cin >> jobs[i].processingTime >> jobs[i].dueDate;
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