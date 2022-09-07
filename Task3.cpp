#include "Task3.h"

std::vector<std::string> Global;
bool map3_done;

int main(int argc, char *argv[]){

     // First command line argument is the name of dirty file
    std::string input_file = argv[1];

    // Second command line argument is the name of the clean file
    std::string output_file= argv[2];

    // Function to filter to dirty file into the clean file.
    task1Filter(input_file,output_file);

    map3_done = false;
    for(int i = 3;i < 16; i++){
        std::string file = "Fifo" + std::to_string(i);
        char file_name[file.length() + 1]; 
        strcpy(file_name,file.c_str());
    
        // Creates Fifo file
        if(mkfifo(file_name, 0777) == -1){
            if(errno != EEXIST){
                perror("Could Not Create Fifo File\n");
            }
        }
    }
      
    // Reads words into global array
    std::ifstream file;
    file.open(output_file);
    std::string line;
    while(getline(file,line)){
        Global.push_back(line);
    }

    // Closes file obtaining words from
    file.close();
    
    // Declares map and reduce threads
    pthread_t map;
    pthread_t reduce;

    // Create Map Thread
    if(pthread_create(&map, nullptr, &map3, nullptr) != 0){
        perror("Failed to create thread\n");
    }
    else{
        printf("Map3 Thread has started\n");
    }

    // Create Reduce Thread
    if(pthread_create(&reduce, nullptr, &reduce3, &output_file) != 0){
        perror("Failed to create thread\n");
    }
   
    // Join Map Thread
    if(pthread_join(map,nullptr)!=0){}
    else{
        printf("Map3 Thread has finished Execution!\n");
    }
    
    printf("Reduce3 Thread has started\n");
    if(pthread_join(reduce, nullptr)!=0){}
    else{
        printf("Reduce3 Thread has finished Execution!\n");
    }
    // Deletes the Fifo Files
    for(int i = 3;i < 16;i++){
        std::string file = "Fifo" + std::to_string(i);
        remove(file.c_str());
    }

    return EXIT_SUCCESS;
}
void *map3(void* args){
  
    // Declares all 13 threads
    pthread_t th[THREAD_NUM];

    // Declares 13 vectors for each thread
    std::vector<int> index[THREAD_NUM];
    int curr_index;

    // CREATES INDEXES
    for (int i = 0; i < (int)Global.size(); i++) {
        int size = int(Global[i].size());
        index[size - 3].push_back(i);
    }

    // CREATE THREADS
    for (int i = 0; i < THREAD_NUM; i++) {
        curr_index = i + 3;
        if (pthread_create(&th[i], nullptr, &pthread_function, &index[i]) != 0) {
            perror("Failed to create thread\n");
        }
        printf("Thread %d has started\n", curr_index);
     
    }

    // JOINS THREADS
    for(int i = 0;i < THREAD_NUM;i++) {
        curr_index = i + 3;
        if (pthread_join(th[i], nullptr) != 0) {}
        printf("Thread %d has finished execution\n", curr_index);
    }

    // declares that mapping is done
    map3_done = true;
    return nullptr;
}
void *reduce3(void* args){
    // gets output file name from parameters
    std::string file_name = *(std::string*)args;
    
    // declares 13 vectors to store words in obtained from fifo files
    std::vector<std::string> lists[13];
    
    // Reads strings from fifos
    for (int i = 0; i < 13; i++) {
        
        // Gets name of fifo file
        std::string file = "Fifo" + std::to_string(i + 3);
        char file_name[file.length() + 1]; 
        strcpy(file_name,file.c_str());
        
        // Opens fifo for reading
        int fd = open(file_name, O_RDONLY);
        char c;
        std::string current;
        
        // Reads fifo character by character
        while (read(fd, &c, sizeof(c)) > 0) {
            if (c == ' ') {
                lists[i].push_back(current);
                current = "";
            } else {
                current += c;
            }
        }
        
        // closes fifo file
        close(fd);
        
        int index = i + 3;
        printf("Message Has Been Read From Pipe For Index %d\n",index);
    }
    printf("All Pipes Have Been Read!\n");
    printf("Now Sorting and Merging List to File...\n");
    std::vector<std::string> all_words;
    
    // Opens file to write out merge and sorted list
    std::ofstream output(file_name);
    
    // Loop to get the first line of each file
    for(int i = 0; i < THREAD_NUM;i++){
        if (!lists[i].empty()){
            all_words.push_back(lists[i][0]);
            lists[i].erase(lists[i].begin());
        }
    }
    
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
        if (!lists[index].empty()){
            all_words.push_back(lists[index][0]);
            lists[index].erase(lists[index].begin());
        }
    }
    printf("Completed!!!\n");
    
    // Close the file that program writes merged list to.
    output.close();
    pthread_exit(EXIT_SUCCESS);

}
void *pthread_function(void *a)
{
    // Gets vector of indexes from parameters
    std::vector <int> list = *(std::vector <int>*)a;
    
    // Gets the size of strings that index points to in global
    int index_size = Global[list[0]].size();
    
    // Name of fifo file to write to
    std::string file = "Fifo" + std::to_string(index_size);
    char file_name[file.length() + 1]; 
    strcpy(file_name,file.c_str());
    
    // Sorts the index vector
    sort(list.begin(),list.end(),sort_cmdGlobal);
    
    // Concatenates all the strings that the index vector points to
    // into one string
    std::string total;
    for (int i : list){
        total += Global[i] + " ";
    }

    // Opens Fifo file for writing
    int fd = open(file_name, O_WRONLY);

    // Writes string to fifo file
    if(write(fd,total.c_str(),total.length()) < 0 ){
        perror("Error Writing to FIFO Files\n");
    }
    else{
        printf("Message Has Been Written to Pipe For Thread %d\n",index_size);
    }

    // Closes fifo file
    close(fd);
    pthread_exit(EXIT_SUCCESS);

}
bool sort_cmdGlobal(int index1, int index2)
{
   return Global[index1].substr(2,-1) < Global[index2].substr(2,-1);
}
