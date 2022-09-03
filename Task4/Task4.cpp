#include "Task4.h"

std::vector<std::string> Global;
bool map4_done;
int map4Counter;
pthread_cond_t cond4;
pthread_mutex_t mutex4;

int main(int argc, char *argv[]){

     // First command line argument is the name of dirty file
    std::string input_file = argv[1];

    // Second command line argument is the name of the clean file
    std::string output_file= argv[2];
    
    pthread_cond_init(&cond4,NULL);
    pthread_mutex_init(&mutex4,NULL);
    map4Counter = 0;

    // FILTERS INPUT FILE
    task1Filter(input_file,output_file);
   

    // READS WORD INTO GLOBAL ARRAY
    std::ifstream file;
    file.open(output_file);
    std::string input;
    while(getline(file,input)){
        Global.push_back(input);
    }
    file.close();

    // CREATES MAP3 and REDUCE3 THREADS
    pthread_t map;
    pthread_t reduce;

    if(pthread_create(&map, nullptr, &map4, nullptr) != 0){
        perror("Failed to create thread\n");
    }
    printf("Map4 Thread has started\n");
    if(pthread_create(&reduce, nullptr, &reduce4, &output_file) != 0){
        perror("Failed to create thread\n");
    }
    printf("Reduce4 Thread has started\n");

    if(pthread_join(map,nullptr)!=0){}
    else{
        printf("Map4 Thread has finished Execution!\n");
    }
    pthread_mutex_lock(&mutex4);
    while(map4Counter < 13){
        pthread_cond_wait(&cond4,&mutex4);
    }
    pthread_mutex_unlock(&mutex4);
    if(pthread_join(reduce, nullptr)!=0){}
    else{
        printf("Reduce4 Thread has finished Execution!\n");
    }

    return EXIT_SUCCESS;
}
void *map4(void* args){
   
    pthread_t th[THREAD_NUM];
    std::vector<int> index_sizes;
    std::vector<int> indexes[THREAD_NUM];
    int curr_index;

    // CREATES INDEXES
    for (int i = 0; i < (int)Global.size(); i++) {
        auto size = int(Global[i].size());
        indexes[size - 3].push_back(i);
    }
    // CREATE THREADS
    for(int i = 0; i < THREAD_NUM;i++){
        int size =indexes[i].size();
        index_sizes.push_back(size);
    }
    
    sort(index_sizes.begin(),index_sizes.end(), sort_indexes);

    std::vector<int> sorted_indexes[THREAD_NUM];

    for(int i = 0; i < THREAD_NUM;i++){
        for(int j = 0; j < THREAD_NUM;j++){
            if((int)indexes[j].size() == index_sizes[i]){
                sorted_indexes[i] = indexes[j];
            }
        }
    }
   
    pthread_attr_t tattr[THREAD_NUM];
    sched_param param;

    for(int i = 0; i < THREAD_NUM;i++){
        pthread_attr_init (&tattr[i]);
        pthread_attr_setschedpolicy(&tattr[i], SCHED_RR);
        param.sched_priority =  sched_get_priority_max(SCHED_RR) - i;
        pthread_attr_setschedparam (&tattr[i], &param);
    }
    for (int i = 0; i < THREAD_NUM; i++) {
        curr_index = Global[sorted_indexes[i][0]].size();
        if (pthread_create(&th[i], &tattr[i], &pthread_function, &sorted_indexes[i]) != 0) {
            perror("Failed to create thread\n");
        }
        printf("Thread %d has started\n", curr_index);
    }
    // JOINS THREADS
    for(int i = 0;i < THREAD_NUM;i++) {
        curr_index = Global[sorted_indexes[i][0]].size();
        if (pthread_join(th[i], nullptr) != 0) {}
        printf("Thread %d has finished execution\n", curr_index);
    }

    return nullptr;
}
void *reduce4(void* args){
  
    // gets output file name from parameters
    std::string file_name = *(std::string*)args;
    
    // declares 13 vectors to store words in obtained from fifo files
    std::vector<std::string> lists[13];
    
    // Reads strings from fifos
    for (int i = 0; i < 13; i++) {
        
        // Gets name of fifo file
        std::string file = "task4files/Fifo" + std::to_string(i + 3);
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
    std::string file = "task4files/Fifo" + std::to_string(index_size);
    char file_name[file.length() + 1]; 
    strcpy(file_name,file.c_str());
    
    // Creates Fifo file
    if(mkfifo(file_name, 0755) == -1){
        if(errno != EEXIST){
            perror("Could Not Create Fifo File\n");
        }
    }
    
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
    
    map4Counter++;
    pthread_cond_signal(&cond4);
    pthread_exit(EXIT_SUCCESS);

}
bool sort_cmdGlobal(int index1, int index2)
{
   return Global[index1].substr(2,-1) < Global[index2].substr(2,-1);
}
