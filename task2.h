#ifndef OSPA1_TASK2_H
#define OSPA1_TASK2_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <algorithm>
#include <sys/wait.h>

void map2(const std::string& filename);
void reduce2();
bool sort_cmd2(std::string s1, std::string s2);

#endif //OSPA1_TASK2_H
