#include "sorts.h"
std::vector<std::string> mergeSort(std::vector<std::string> Arr, int lo, int hi)
{
    if (lo == hi) {
        std::vector<std::string> A = { Arr[lo] };
        return A;
    }
    int mid = lo + (hi - lo) / 2;
    std::vector<std::string> arr1 = mergeSort(Arr, lo, mid);
    std::vector<std::string> arr2 = mergeSort(Arr, mid + 1, hi);
 
    std::vector<std::string> arr3 = merge(arr1, arr2);
    return arr3;
}
std::vector<std::string> merge(std::vector<std::string> Arr1,std::vector<std::string> Arr2)
{
    int m = Arr1.size();
    int n = Arr2.size();
    std::vector<std::string> Arr3;
 
    int idx = 0;
 
    int i = 0;
    int j = 0;
    while (i < m && j < n) {
        if (isAlphabeticallySmaller(Arr1[i], Arr2[j])) {
            Arr3.push_back(Arr1[i]);
            i++;
            idx++;
        }
        else {
            Arr3.push_back(Arr2[j]);
            j++;
            idx++;
        }
    }
    while (i < m) {
        Arr3.push_back(Arr1[i]);
        i++;
        idx++;
    }
    while (j < n) {
        Arr3.push_back(Arr2[j]);
        j++;
        idx++;
    }
    return Arr3;
}
bool isAlphabeticallySmaller(std::string str1, std::string str2){
    transform(str1.begin(), str1.end(), str1.begin(),
              ::toupper);
    transform(str2.begin(), str2.end(), str2.begin(),
              ::toupper);
    if (str1.substr(2,-1) < str2.substr(2,-1)) {
        return true;
    }
    return false;
}
bool sort_string(std::string s1, std::string s2) {
    std::string i1=  s1.substr(2,-1);
    std::string i2=  s2.substr(2,-1);
    return i1 < i2;
}