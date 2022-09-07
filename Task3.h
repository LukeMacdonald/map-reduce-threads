#ifndef OSPA1_TASK3_H
#define OSPA1_TASK3_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <algorithm>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "Task1Filter.h"
#include "Sorts.h"


#define THREAD_NUM 13

int main(int argc, char *argv[]);
void *map3(void *a);
void *reduce3(void *a);
void *pthread_function(void *a);
bool sort_cmdGlobal(int s1, int s2);

#endif //OSPA1_TASK3_H
