#include "Utils.h"

void pop_front(std::vector<std::string> &v)
{
    if (v.size() > 0)
    {
        v.erase(v.begin());
    }
}
bool sort_string(std::string str1, std::string str2) {
    return str1.substr(2,-1) < str2.substr(2,-1);
}
bool sort_indexes(int i1, int i2) {
    return i1 > i2;
}