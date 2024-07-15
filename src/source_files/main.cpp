#include <iostream>
#include <filesystem>
#include <vector>
#include <climits>
#include <utility>
#include <string>
#include <chrono>
#include "../header_files/Job.h"
#include "../header_files/utilities.h"
#include "../header_files/exact.h"
#include "../header_files/tinder_search.h"

using namespace std;
using namespace chrono;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Usage: ./TINDER_SEARCH <path_to_benchmarks>" << endl;
        return -1;
    } 
    string path = argv[1];

    auto now = high_resolution_clock::now();
	auto timeMillis = duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
	srand(timeMillis);

    vector<filesystem::directory_entry> entries;

    for (const auto & entry : filesystem::directory_iterator(path)) {
        entries.push_back(entry);
    }

    sort(entries.begin(), entries.end(), [](const filesystem::directory_entry &a, const filesystem::directory_entry &b) {
        return a.path().filename().string() < b.path().filename().string();
    });

    for (const auto & entry : entries) {
        // store the information of the file
        int n, m;
        vector<Job*> jobs;
        int flag = read_fileinput(entry.path(), jobs, n, m);

        if (flag == -1) {
            cout << "Error reading the file " << entry.path() << endl;
            return -1;
        }
 
        high_resolution_clock::time_point start, end;
        vector<vector<Job*>> schedule;
        start = high_resolution_clock::now();
        // schedule = tinder_search(jobs, m, 500, 0.05, 20, 10, 0.05, true, 5);
        // increase the number of profiles to show
        // schedule = tinder_search(jobs, m, 500, 0.05, 20, 100, 0.05, true, 5);
        // increase the delta value
        // schedule = tinder_search(jobs, m, 500, 0.05, 20, 100, 0.25, true, 5);
        // set false to ghost
        // schedule = tinder_search(jobs, m, 500, 0.05, 20, 100, 0.25, false, 5);
        // increase the number of messages
        schedule = tinder_search(jobs, m, 500, 0.05, 20, 100, 0.25, true, 15);
        end = high_resolution_clock::now();
        duration<double> duration = duration_cast<chrono::duration<double>>(end - start);

        cout << "❤️‍🔥 Tinder Search Schedule" << endl;
        print_schedule(schedule, m);

        // calculate total tardiness
        long long tardiness = total_tardiness(schedule);
        cout << "\nTotal Tardiness: " << tardiness << endl;

        cout << "Time taken by tinder search ";
        cout << duration.count() << " seconds\n" << endl;
    }

    return 0;
}