#include "Task5.h"

#define BUFFER 250
OutputHandler output_handler;

int main(int argc, char *argv[]){
    clock_t start,end;
    start = clock();
    output_handler.open_files("Task5Files/");

    std::string output_file= argv[1];
    
    if(mkfifo("Task5Files/Fifo/Map5", 0777) == -1){
            if(errno != EEXIST){
                output_handler.print_error("Could Not Create Fifo File\n");
            }
    }
    for(int i = 3;i < 16; i++){
        std::string file = "Task5Files/Fifo/Fifo" + std::to_string(i);
        char file_name[file.length() + 1]; 
        strcpy(file_name,file.c_str());
    
        // Creates Fifo file
        if(mkfifo(file_name, 0777) == -1){
            if(errno != EEXIST){
                output_handler.print_error("Could Not Create Fifo File\n");
            }
        }
    }
    pthread_t map;
    pthread_t reduce;
    std::string line;
    bool valid;
    pthread_create(&reduce,nullptr,&reduce5,&output_file);
    pthread_create(&map,nullptr,&map5,nullptr);
    int fd = open("Task5Files/Fifo/Map5",O_WRONLY);
    output_handler.print_log("Reading Words in Vectors From Server Stream!");
    while (std::getline(std::cin, line))
    {
        int size = line.length();
        if(size > 2 && size < 16){
            valid = true;
            for (int i = 0; i < size; i++){
                if(!std::isalpha(line[i])){
                    valid = false;
                }
            }
            if(valid){
                if(write(fd,line.c_str(),line.length() + 1)<0){
                    output_handler.print_error("Error Writting to File!");
                };
            }
        }
    }
    close(fd);
    pthread_join(map,nullptr);
    pthread_join(reduce,nullptr);
    end = clock();
    std::cout << "Task 5 Finish Completed!\nOutput File is located in in 'Task5Files/output' directory as '" 
    << output_file <<  "'\n";
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_handler.print_exec_time("Total",time_taken);
    output_handler.close_files();
    return EXIT_SUCCESS;
}
void* map5(void* args){
    int cd[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) {
        std::string file = "Task5Files/Fifo/Fifo" + std::to_string(i + 3);
        char file_name[file.length() + 1]; 
        strcpy(file_name,file.c_str());
        cd[i] = open(file_name,O_WRONLY);
    }
    char line[BUFFER];
    std::string c;
    int fd = open("Task5Files/Fifo/Map5",O_RDONLY);
    while(read(fd,&line,BUFFER)>0){
        c = line;
        int index = c.length() - 3;
        if(write(cd[index],c.c_str(),c.length() + 1)>0){

        };
    }
    close(fd);
    for (int i = 0; i < THREAD_NUM; i++) {
        close(cd[i]);
    }
    pthread_exit(EXIT_SUCCESS);
}
void* reduce5(void* args){
    
     // Declares all 13 threads
    pthread_t th[THREAD_NUM];
    std::vector<std::string> v[THREAD_NUM];
    // CREATE THREADS
    for (int i = 0; i < THREAD_NUM; i++) {
        v[i].push_back(std::to_string(i + 3));
        if (pthread_create(&th[i], nullptr, &pthread_function, &v[i]) != 0) {
            output_handler.print_error("Failed to create thread\n");
        }
    }
    // JOINS THREADS
    for(int i = 0;i < THREAD_NUM;i++) {
        if (pthread_join(th[i], nullptr) != 0) {}
    }
    std::vector<std::string> all_words;
    for(int i = 0; i < THREAD_NUM;i++){
        all_words.push_back(v[i][0]);
        v[i].erase(v[i].begin());
    }

    // Opens file to write out merge and sorted list
    std::string file_name = "Task5Files/output/" + *(std::string*)args;
    std::ofstream output(file_name);
    // Performs sorting and writing on all strings from fifo files
    while (!all_words.empty()){
        
        // Sorts all words in vector from smallest to largest based on the third character onwards
        std::sort(all_words.begin(),all_words.end(),sort_string);
        
        // Writes smallest word out to file
        output << all_words[0] << std::endl;
        
        // Gets which file the smallest word came from
        int index = all_words[0].size() - 3;
        
        // Removes smallest word from vector
        all_words.erase(all_words.begin());
        
        // Adds the next word from the file that the smallest word came from
        // into the vector only if there is one there.
        if (!v[index].empty()){
            all_words.push_back(v[index][0]);
            v[index].erase(v[index].begin());
        }
    }
    output_handler.print_error("Task 5 Completed!!!\n");
    
    // Close the file that program writes merged list to.
    output.close();
    
    pthread_exit(EXIT_SUCCESS);
}
void *pthread_function(void *a){
    
    std::vector<std::string>&v = *(std::vector<std::string>*)a;
    
    std::string file = "Task5Files/Fifo/Fifo" + v[0];
    
    int fd = open(file.c_str(),O_RDONLY);
    
    v.erase(v.begin());
    
    char line[BUFFER];
    
    while(read(fd,&line,BUFFER)>0){
        v.push_back(line);
    }
    close(fd);
    sort(v.begin(),v.end(),sort_string);
    pthread_exit(EXIT_SUCCESS);
}
