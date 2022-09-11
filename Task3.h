#ifndef OSPA1_TASK3_H
#define OSPA1_TASK3_H

#include <string>
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <fstream>
#include "Task1Filter.h"
#include "Structures.h"
#include "Utils.h"
#include "OutputHandler.h"

// mapping method for task3
void* map3(void* args);
// reduce method for task3
void* reduce3(void* args); 
// method for writing data to fifo file
void *pthread_write(void *args);
// method for reading data from fifo file
void *pthread_read(void *args);
// method for sorting global vector
bool sort_global(int index1, int index2);
// method for handling graceful exit to prevent
// thread starvation
void graceful_exit_handler(int seconds);

#endif //OSPA1_TASK3_H