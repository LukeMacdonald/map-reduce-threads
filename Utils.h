#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <fcntl.h>
#include <chrono>
#include <algorithm>
#include <queue>
#include <fstream>


#define THREAD_NUM 13
#define PROCESS_NUM 13
#define ARRAY_OFFSET 3

void pop_front(std::vector<std::string> &v);
bool sort_string(std::string str1, std::string str2);
bool sort_indexes(int i1, int i2);