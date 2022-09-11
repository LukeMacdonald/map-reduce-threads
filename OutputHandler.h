#ifndef OSPA1_OUTPUTHANDLER_H
#define OSPA1_OUTPUTHANDLER_H

#include <iostream>
#include <fstream>
#include <unistd.h>

class OutputHandler{
    public:
        // Constructor for OutputHandler
        OutputHandler();
        // Deconstructor for OutputHandler
        ~OutputHandler();
        // method for opening all files that OutputHandler
        // handles
        void open_files(std::string task);
        // method for closing all files that OutputHandler
        // handles
        void close_files();
        // method for printing the stdout of program to
        // log file
        void print_log(std::string print);
        // method for printing the sterror of program to
        // error file
        void print_error(std::string print);
        // method for printing the tim exectution of tasks to
        // time file
        void print_exec_time(std::string program,double time);
    private:
        std::ofstream error_file;
        std::ofstream log_file;
        std::ofstream time_recorder;
};
#endif //OSPA1_OUTPUTHANDLER_H