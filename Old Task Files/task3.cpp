#include "task3.h"
std::vector<std::string> Global;
int map3Counter;
pthread_cond_t cond3;
pthread_mutex_t mutex3;
void main_task3(const std::string& dirty_file,const std::string& clean_file){

    pthread_cond_init(&cond3,NULL);
    pthread_mutex_init(&mutex3,NULL);


    // FILTERS INPUT FILE
    task1filter(dirty_file,clean_file);
    map3Counter = 0;
  
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

    pthread_mutex_lock(&mutex3);
    while(map3Counter < 13){
        pthread_cond_wait(&cond3,&mutex3);
    }
    pthread_mutex_unlock(&mutex3);

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

    return nullptr;
}
void *reduce3(void* args){
  
    std::vector<std::string> lists[13];
    for (int i = 0; i < 13; i++) {
        std::string file = "task3files/Fifo" + std::to_string(i + 3);
        
        char file_name[file.length() + 1]; 
        strcpy(file_name,file.c_str());
        // FILE * fp;
        // fp = fopen (file_name , "r");
        // int c;
        // std::string current;
        // while(!feof(fp)){
        //      c = fgetc(fp);
        //     if(c == ' '){
        //         lists[i].push_back(current);
        //         current = "";
        //     }
        //     else{
        //         current+=c;
        //     }
        // }
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
    
    std::vector<std::string> lists_words;
    std::ofstream output;
    printf("Now Sorting and Merging List to File...\n");
    output.open("task3files/merge.txt");
    
    for(int j = 0; j < THREAD_NUM;j++){
        lists_words.push_back(lists[j][0]);
        lists[j].erase(lists[j].begin());
    }
    int size = lists_words.size() - 1;

    lists_words = mergeSort(lists_words,0,size);

    for (int i = 0; i < Global.size();i++){
        output << lists_words[0] << std::endl;
        int next = lists_words[0].size() - 3;
        lists_words.erase(lists_words.begin());
        if(!lists[next].empty()){
            lists_words.push_back(lists[next][0]);
            lists[next].erase(lists[next].begin());
            size = lists_words.size() - 1;
            lists_words = mergeSort(lists_words,0,size);
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
            perror("Could Not Create Fifo File");
        }
    }
    sort(list.begin(),list.end(),sort_cmdGlobal);
    std::string total;
    for (int i : list){
        total += Global[i] + " ";
    }
    //  FILE * fp;
    //  fp = fopen (file_name , "w");
    //  int val = fwrite(total.c_str(),total.length()+1,1,fp);
    //  std::cout << val;
    
    int fd = open(file_name, O_WRONLY);
    if(write(fd,total.c_str(),total.length()) < 0 ){
        perror("Error Writing to FIFO Files");
    }
    else{
        printf("Message Has Been Written to Pipe For Thread %d\n",index_size);
    }
    close(fd);
    map3Counter++;
    pthread_cond_signal(&cond3);

    return nullptr;

}
bool sort_cmdGlobal(int index1, int index2)
{
    return Global[index1].substr(2,-1)< Global[index2].substr(2,-1);
}