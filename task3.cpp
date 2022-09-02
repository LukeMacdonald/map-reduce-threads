#include "task3.h"

std::vector<std::string> Global;
bool map3_done;

void main_task3(const std::string& dirty_file,const std::string& clean_file){

    // FILTERS INPUT FILE
    task1filter(dirty_file,clean_file);
    map3_done = false;
  
    // READS WORD INTO GLOBAL ARRAY
    std::ifstream file;
    file.open(clean_file);
    std::string input;
    while(getline(file,input)){
        Global.push_back(input);
    }
    file.close();

    // CREATES MAP3 and REDUCE3 THREADS
    pthread_t map;
    pthread_t reduce;

    if(pthread_create(&map, nullptr, &map3, nullptr) != 0){
        perror("Failed to create thread\n");
    }
    printf("Map3 Thread has started\n");
    if(pthread_create(&reduce, nullptr, &reduce3, nullptr) != 0){
        perror("Failed to create thread\n");
    }
    
    printf("Reduce3 Thread has started\n");

    if(pthread_join(map,nullptr)!=0){}
    else{
        printf("Map3 Thread has finished Execution!\n");
    }

    while(!map3_done){}
    if(pthread_join(reduce, nullptr)!=0){}
    else{
        printf("Reduce3 Thread has finished Execution!\n");
    }
}
void *map3(void* args){
  

    pthread_t th[THREAD_NUM];
    std::vector<int> index[THREAD_NUM];
    int curr_index;

    // CREATES INDEXES
    for (int i = 0; i < (int)Global.size(); i++) {
        auto size = int(Global[i].size());
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
    map3_done = true;
    return nullptr;
}
void *reduce3(void* args){
  
    std::vector<std::string> lists[13];
    for (int i = 0; i < 13; i++) {
        std::string file = "task3files/Fifo" + std::to_string(i + 3);
        
        char file_name[file.length() + 1]; 
        strcpy(file_name,file.c_str());
        int fd = open(file_name, O_RDONLY);
        char c;
        std::string current;
        while (read(fd, &c, sizeof(c)) > 0) {
            if (c == ' ') {
                lists[i].push_back(current);
                current = "";
            } else {
                current += c;
        
            }
        }
        close(fd);
        int index = i + 3;
        printf("Message Has Been Read From Pipe For Index %d\n",index);
    }
    printf("All Pipes Have Been Read!\n");
    int max = 0;
    for (int i = 0 ; i < THREAD_NUM;i++) {
        if ((int)lists[i].size() > max){
            max = (int)lists[i].size();
        }
    }
    
    std::vector<std::string> lists_words;
    std::ofstream output;
    printf("Now Sorting and Merging List to File...\n");
    output.open("task3files/merge.txt");
     for(int j = 0; j < THREAD_NUM;j++){
        lists_words.push_back(lists[j][0]);
        lists[j].erase(lists[j].begin());
    }
    for (int i = 0; i < Global.size();i++){
        sort(lists_words.begin(), lists_words.end(), sort_cmd3);
        output << lists_words[0] << std::endl;
        int next = lists_words[0].size() - 3;
        lists_words.erase(lists_words.begin());
        if(!lists[next].empty()){
            lists_words.push_back(lists[next][0]);
            lists[next].erase(lists[next].begin());
        }
    }
    printf("Completed!!!\n");

    output.close();
    return nullptr;

}
void *pthread_function(void *a)
{
    
    std::vector <int> list = *(std::vector <int>*)a;
    int index_size = Global[list[0]].size();
    std::string file = "task3files/Fifo" + std::to_string(index_size);
    
    char file_name[file.length() + 1]; 
    strcpy(file_name,file.c_str());
    
    if(mkfifo(file_name, 0755) == -1){
        if(errno != EEXIST){
            perror("Could Not Create Fifo File\n");
        }
    }
    sort(list.begin(),list.end(),sort_cmdGlobal);
    std::string total;
    for (int i : list){
        total += Global[i] + " ";
        
    }
    
    int fd = open(file_name, O_WRONLY);
    if(write(fd,total.c_str(),total.length()) < 0 ){
        perror("Error Writing to FIFO Files\n");

    }
    else{
        printf("Message Has Been Written to Pipe For Thread %d\n",index_size);
    }
    close(fd);
    return nullptr;

}
bool sort_cmdGlobal(int s1, int s2)
{
    std::string i1=  Global[s1].substr(2,-1);
    std::string i2=  Global[s2].substr(2,-1);
    return i1 < i2;
}
bool sort_cmd3(std::string s1, std::string s2) {
    std::string i1=  s1.substr(2,-1);
    std::string i2=  s2.substr(2,-1);
    return i1 < i2;
}
  
