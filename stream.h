#ifndef OSPA1_STREAM_H
#define OSPA1_STREAM_H
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

class Stream{
public:
    Stream(const std::string& filename);
    std::string get();
    int size();
private:
    std::vector<std::string> words;
    int length;
};
#endif //OSPA1_STREAM_H
