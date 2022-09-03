#ifndef OSPA1_SORTS_H
#define OSPA1_SORTS_H

#include <iostream>
#include <vector>

std::vector<std::string> mergeSort(std::vector<std::string> Arr, int lo, int hi);
std::vector<std::string> merge(std::vector<std::string> Arr1,std::vector<std::string> Arr2);
bool isAlphabeticallySmaller(std::string str1, std::string str2);
bool sort_string(std::string s1, std::string s2);

#endif //OSPA1_SORTS_H