#ifndef OSPA1_TASK3_H
#define OSPA1_TASK3_H

#include <string>
#include <pthread.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fstream>
#include "Task1Filter.h"
#include "Structures.h"
#include "Utils.h"
#include "OutputHandler.h"

void* map4(void* args);
void* reduce4(void* args); 
void *pthread_write(void *args);
void *pthread_read(void *args);
bool sort_global(int index1, int index2);
void graceful_exit_handler(int seconds);

#endif //OSPA1_TASK3_H