#include "Task5.h"
#include <fcntl.h>
#include <unistd.h>
#include "sorts.h"

#define BUFFER 20

bool completed;
bool turn;



int main(int argc, char *argv[]){
    std::string input_file = argv[1];
    std::string output_file= argv[2];
    Stream* s = new Stream(input_file);

    pthread_t map5;
    pthread_t reduce5;
    completed = false;
    turn = true;


    pthread_create(&map5,NULL,&map,&s);
    pthread_create(&reduce5,NULL,&reduce,&output_file);

    pthread_join(map5,NULL);
    pthread_join(reduce5,NULL);

   
}
void* map(void* args){
   
    std::string fifo = "Fifo5";
    mkfifo(fifo.c_str(),0777);
    
    std::string current;
    Stream* s = *(Stream**)args;
    int total = s->size();
    bool valid;
    for(int i = 0; i < total;i++){
        while(!turn){}
        current = s->get();
        valid = true;
        int word_size = current.size();
        if (word_size>2 && word_size < 16){
            for(int j = 0; j < current.size();j++){
                if(!std::isalpha(current[j])){
                    valid = false;
                }
            }
            if(valid){
                int fd = open(fifo.c_str(),O_WRONLY);
                write(fd,current.c_str(),current.length() + 1);
                close(fd);
                turn = false;
            }
        }
    }
    completed = true;

    pthread_exit(EXIT_SUCCESS);
}
void* reduce(void* args){
    std::string file_name = *(std::string*)args;
    std::vector <std::string> lists;
    std::ofstream output;
    

    std::string fifo = "Fifo5";
    char arr[BUFFER];
    while(!completed){
        while(turn){}
        int fd = open(fifo.c_str(),O_RDONLY);
        read(fd,arr,BUFFER);
        lists.push_back(arr);
        close(fd);
        turn = true;
    }
    output.open(file_name);
    int size = lists.size();
    lists = mergeSort(lists,0,size - 1);
    for (int i = 0; i < (int)lists.size();i++){
        output << lists[i] << std::endl;
    }
    output.close();
    pthread_exit(EXIT_SUCCESS);
    

}