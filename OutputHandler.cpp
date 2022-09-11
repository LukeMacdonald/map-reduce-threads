#include "OutputHandler.h"
#include <iomanip>
#include <ctime>
#include <sstream>

OutputHandler::OutputHandler(){}
OutputHandler:: ~OutputHandler(){}
void OutputHandler::open_files(std::string task){
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream datetime;
    datetime << std::put_time(&tm, "%d-%m-%Y#%H-%M-%S");
    
    std::string error_filename = task + "/errors/" + datetime.str() + ".txt";
    std::string log_filename = task + "/logs/" + datetime.str() + ".txt";
    std::string time_filename = task + "/times/" + datetime.str() + ".txt";
   
    std::cout << "Console Output Redirected To: " << log_filename << std::endl;
    std::cout << "Error Output Redirected To: " << error_filename << std::endl;
    std::cout << "Execution Time's of Program Output Redirected To: " << time_filename << std::endl;
   
    error_file.open(error_filename);
    error_file << "PID   |   ERROR\n";
    log_file.open(log_filename);
    log_file << "PID   |   OUTPUT\n";
    for(int i = 0;i < 20;i++){
        log_file << "--";
        error_file << "--";
    }
    log_file<<'\n';
    error_file << '\n';
    time_recorder.open(time_filename);
}
void OutputHandler::close_files(){
    error_file.close();
    log_file.close();
    time_recorder.close();
}
void OutputHandler::print_log(std::string print)
{
    log_file << std::to_string(getpid()) + " | " + print + "\n";
    log_file.flush();
}
void OutputHandler::print_error(std::string print)
{
    error_file << std::to_string(getpid()) + " | " + print + "\n";
    error_file.flush();
}
void OutputHandler::print_exec_time(std::string program,double time)
{
    time_recorder << std::left << std::setw(15) << program <<  " | " << std::fixed 
         << time << std::setprecision(5) << " seconds\n";
    time_recorder.flush();
}
