#ifndef ACO_H
#define ACO_H

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include "heuristic.h"
#include "Job.h"
#include "exact.h"
#include "local_search.h"

using namespace std;

vector<vector<Job*>> ACO(vector<Job*> jobs, int machines, int iterations, int ants, 
                        int alpha, int beta, double qm0, double qj0, double rho_local, double rho_global);

#endif // ACO_H