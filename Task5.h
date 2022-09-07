#ifndef TASK5_H
#define TASK5_H

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



int main(int argc, char *argv[]);
void* map5(void* args);
void* reduce5(void* args);
void *pthread_function(void *a);

#endif //TASK5_H