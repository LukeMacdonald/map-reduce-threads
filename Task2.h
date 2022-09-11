#ifndef OSPA1_TASK2_H
#define OSPA1_TASK2_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <algorithm>
#include <sys/wait.h>
#include "Task1Filter.h"
#include "Utils.h"
#include "OutputHandler.h"

#define PROCESS_NUM 13

// Mapping method for task2
void map2(std::string filename);
// Reduce method for task2
void reduce2(std::string filename);
// Function that does the mapping of the 13 different lengths
// and is called by the child process
void child_function(int index,std::vector<std::string> list);

#endif //OSPA1_TASK2_H
