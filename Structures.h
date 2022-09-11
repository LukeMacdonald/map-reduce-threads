#include <iostream>
#include <queue>
struct FIFOThreadParams {
    int index;
    std::queue<std::string>* stored_strings; 
};
struct MapParams {
    int word_length;
    std::vector<int> indexes;
};