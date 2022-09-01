
#include "task1.h"
#include "task2.h"

int main(int argc, char *argv[]) {
    std::string task = argv[1];
    std::string input_file = argv[2];
    std::string output_file= argv[3];
    if (task == "task1"){
        task1filter(input_file,output_file);
    }
    else if (task == "task2"){
        task1filter(input_file,output_file);
        map2(output_file);
    }
  
  
    return 0;
}
