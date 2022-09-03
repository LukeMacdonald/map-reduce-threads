#ifndef TASK2_H
#define TASK2_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <algorithm>
#include <sys/wait.h>
#include "Task1.h"
#include "Sorts.h"

#define PROCESS_NUM 13

int main(int argc, char *argv[]);
void map2(std::string filename);
void reduce2(std::string filename);
void child_function(int index,std::vector<std::string> list);

#endif //TASK2_H
