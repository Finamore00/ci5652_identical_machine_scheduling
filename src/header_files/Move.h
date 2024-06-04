#ifndef MOVE_H
#define MOVE_H

#include <functional>

struct Move {
    int machine_index;
    int job_index_1;
    int job_index_2;

    Move(int m, int j1, int j2) : machine_index(m), job_index_1(j1), job_index_2(j2) {}

    bool operator==(const Move& other) const {
        return machine_index == other.machine_index &&
               (job_index_1 == other.job_index_1 ||
               job_index_2 == other.job_index_2);
    }
};

struct MoveHasher {
    std::size_t operator()(const Move& move) const {
        return std::hash<int>()(move.machine_index) ^
               std::hash<int>()(move.job_index_1) ^
               std::hash<int>()(move.job_index_2);
    }
};

#endif // MOVE_H