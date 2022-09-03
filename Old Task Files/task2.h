#ifndef OSPA1_TASK2_H
#define OSPA1_TASK2_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <algorithm>
#include <sys/wait.h>
#include "sorts.h"

#define PROCESS_NUM 13

void map2(const std::string& filename);
void reduce2();
void child_function(int index,std::vector<std::string> list);

#endif //OSPA1_TASK2_H
