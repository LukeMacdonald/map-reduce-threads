#ifndef OSPA1_STRUCTURES_H
#define OSPA1_STRUCTURES_H

#include <iostream>
#include <queue>

struct ReduceParams {
    int index;
    std::queue<std::string>* stored_strings; 
};
struct MapParams {
    int word_index;
    std::vector<int> indexes;
};

#endif //OSPA1_STRUCTURES_H