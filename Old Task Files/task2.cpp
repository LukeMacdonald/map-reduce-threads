#include "task2.h"
void map2(const std::string& filename){
    
    std::string input;
    std::ifstream input_file;
    input_file.open(filename);
    std::vector<std::string> index[13];

    while (std::getline(input_file, input)) {
        auto size = int(input.size());
        index[size - 3].push_back(input);
    }
    input_file.close();

    int index_size;
    
    for (int i = 0; i < PROCESS_NUM; i++) {
        if (fork()== 0) {
            index_size = i + 3;
            child_function(index_size,index[i]);
            exit(0);
        }
    }
    for (int i = 0; i <= PROCESS_NUM; i++) {
        wait(nullptr);
    }
    reduce2();
    
}
void reduce2(){
    std::ifstream files[13];
    std::string words[13];
    std::vector<std::string> lists[PROCESS_NUM];

    for (int i = 0; i < 13; i++){
        files[i].open("task2files/File" + std::to_string(i + 3) + ".txt");
        words[i] = "";
    }
    do
    {
        for (int i = 0; i < 13; i++ ){
            if (std::getline(files[i], words[i])){
                lists[i].push_back(words[i]);
            }
            else{
                words[i] = "";
            }
        }
    }while (!words[0].empty() || !words[1].empty() || !words[2].empty() || !words[3].empty() ||
            !words[4].empty() || !words[5].empty() || !words[6].empty() || !words[7].empty() ||
            !words[8].empty() || !words[9].empty() || !words[10].empty() || !words[11].empty() ||
            !words[12].empty());
    
    std::vector<std::string> lists_words;
    std::ofstream output;
    printf("Now Sorting and Merging List to File...\n");
    output.open("task2files/merge.txt");
    for (int i = 0; i < 13; i++){
        files[i].close();
    }
    int total = 0;
    for(int j = 0; j < PROCESS_NUM;j++){
        lists_words.push_back(lists[j][0]);
        total += lists[j].size();
        lists[j].erase(lists[j].begin());
        
    }
    int size = lists_words.size() - 1;

    lists_words = mergeSort(lists_words,0,size);

    for (int i = 0; i < total;i++){
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
    output.close();
}
void child_function(int index,std::vector<std::string> list){
    std::ofstream file;
    sort(list.begin(),list.end(), sort_string);
    file.open("task2files/File" + std::to_string(index) + ".txt");
    for (std::string &line: list) {
        file << line << std::endl;
    }
    file.close();
}