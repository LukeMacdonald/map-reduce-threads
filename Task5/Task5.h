#ifndef TASK5_H
#define TASK5_H

#include <iostream>
#include <fstream>
#include <pthread.h>
#include <sys/stat.h>
#include <string.h>
#include "stream.h"


#define THREAD_NUM 13

int main(int argc, char *argv[]);
void* map(void* args);
void* reduce(void* args);

#endif //TASK5_H