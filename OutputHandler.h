#include <iostream>
#include <fstream>
#include <unistd.h>
class OutputHandler{
    public:
        OutputHandler();
        ~OutputHandler();
        void open_files(std::string task);
        void close_files();
        void print_log(std::string print);
        void print_error(std::string print);
        void print_exec_time(std::string program,double time);
    private:
        std::ofstream error_file;
        std::ofstream log_file;
        std::ofstream time_recorder;
};