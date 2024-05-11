#ifndef UTILITIES
    #define UTILITIES
    
    #include <string>
    #include <vector>
    #include <fstream>
    #include <iostream>
    #include "Job.h"

    using namespace std;

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

    int read_fileinput(filesystem::path path, vector<Job>& jobs, int& num_jobs, int& num_machines) {
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
                Job job = {idjob, process_time, due_date};
                jobs.push_back(job);
            }
        }
        file.close(); // close the file
        return 1; // success
        }
    }

    

#endif