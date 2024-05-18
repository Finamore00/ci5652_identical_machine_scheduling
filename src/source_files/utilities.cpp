#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "../header_files/Job.h"

using namespace std;

/*
    Split a string into a vector of integers.
    Args:
        - str is the string to be split
    Returns:
        - a vector of integers
*/
vector<int> splitstring(string str)
{
    vector<int> v;
    string word = "";
    for (auto x : str) 
    {
        if (x == ' ')
        {
            if (word != "")
            {
                v.push_back(stoi(word));
                word = "";
            }
        }
        else {
            word = word + x;
        }
    }
    if (word != "")
        v.push_back(stoi(word));
    return v;
}

/*
    Read the input file and store the jobs in a vector, the number of jobs, and the number of machines.
    Args:
        - path is the path to the input file
        - jobs is a vector of jobs
        - num_jobs is the number of jobs
        - num_machines is the number of machines
    Returns:
        - 1 if the file is read successfully
        - -1 if the file is not found
*/
int read_fileinput(filesystem::path path, vector<Job*>& jobs, int& num_jobs, int& num_machines) {
    ifstream file(path);
    // check if the file is found
    if (!file) {
        cout << "File not found" << endl;
        return -1; // error
    }

    string line;

    bool storedNM = false; // is n and m already stored?
    int idjob; long long process_time, due_date;

    if (file.is_open()) {
        // read the file line by line
        while (std::getline(file, line)) {
            // ignore the lines beginning with #
            if (line.front() == '#')
                continue;

            // split the line into a vector of integers
            vector<int> v = splitstring(line);

            if (!storedNM) {
                // store n and m
                num_jobs = v[0]; num_machines = v[1];
                storedNM = true;
            }
            else {
                // store the job id, processing time, and due date
                idjob = v[0]; process_time = v[1]; due_date = v[2];
                Job *job = new Job();
                job->id = idjob;
                job->ptime = process_time;
                job->dDate = due_date;
                jobs.push_back(job);
            }
        }
        file.close(); // close the file
    }

    return 1; // sucess
}

void print_schedule(vector<vector<Job*>> schedule, int machine_count) {
    for (int i = 0; i < machine_count; i++) {
        cout << "\nMachine " << i + 1 << ": ";
        for (Job *job : schedule[i]) {
            cout << job->id << " ";
        }
        cout << endl;
    }
}