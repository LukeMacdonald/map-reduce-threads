#ifndef UNTITLED_TASK3_H
#define UNTITLED_TASK3_H

#include <iostream>
#include <fstream>
#include "task1.h"
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <algorithm>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#define THREAD_NUM 13

void main_task3(const std::string& dirty_file,const std::string& clean_file);
void *map3(void *a);

void *reduce3(void *a);
void *pthread_function(void *a);
void *pthread_reduce(void *a);
bool sort_cmdGlobal(int s1, int s2);
bool sort_cmd3(std::string s1, std::string s2);

#endif //UNTITLED_TASK3_H
