#ifndef STRUCT_TABU_LIST
#define STRUCT_TABU_LIST

#include <vector>
#include <utility>

struct tabu_list {
    std::vector<std::pair<int, int>> l;
    int tail;

    tabu_list() : l(std::vector<std::pair<int, int>>(10, std::pair(-1,-1))), tail(0) {}

    void add(std::pair<int, int> a) {
        l[tail] = a;
        if(tail == 9) {
            tail = 0;
        } else {
            tail = tail + 1;
        }
    }

    bool belongs(int a, int b) {
        bool flag = false;
        for (size_t i = 0; i < l.size(); i++) {
            if (a == l[i].first && b == l[i].second) {
                flag = true;
                break;
            }
        }
        return flag;
    }
};

#endif