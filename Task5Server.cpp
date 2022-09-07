#include "Task5Server.h"

int main(int argc, char *argv[]){
    
    std::string input_file = argv[1];
    
    Stream* s = new Stream(input_file);
    
    int size = s->size();
    
    for(int i = 0; i < size;i++){
        std::cout << s->get() << std::endl;
    }
}

