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

    for (int i = 3; i <= 15; i++) {
        int pid = fork();
        if (pid == 0) {
            std::ofstream f1;
            sort(index[i - 3].begin(),index[i - 3].end(), sort_cmd2);
            f1.open("task2files/File" + std::to_string(i) + ".txt");
            for (std::string &line: index[i - 3]) {
                f1 << line << std::endl;
            }
            f1.close();

        }
    }
    wait(nullptr);
    //reduce2();
}
void reduce2(){
    std::ifstream files[13];
    std::string words[13];
    std::string final_file = "merge.txt";
    std::ofstream output;
    std::vector<std::string> lists;

    for (int i = 0; i < 13; i++){
        files[i].open("task2files/File" + std::to_string(i + 3) + ".txt");
        words[i] = "";
    }
    output.open(final_file);
    do
    {
        for (int i = 0; i < 13; i++ ){
            if (std::getline(files[i], words[i])){
                lists.push_back(words[i]);
            }
            else{
                words[i] = "";
            }
        }
        sort(lists.begin(), lists.end(), sort_cmd2);
        for(std::string & line : lists){
            output << line << std::endl;
        }
        lists.clear();
    }while (!words[0].empty() || !words[1].empty() || !words[2].empty() || !words[3].empty() ||
            !words[4].empty() || !words[5].empty() || !words[6].empty() || !words[7].empty() ||
            !words[8].empty() || !words[9].empty() || !words[10].empty() || !words[11].empty() ||
            !words[12].empty());
    output.close();
    for (int i = 0; i < 13; i++){
        files[i].close();
    }
}
bool sort_cmd2(std::string s1, std::string s2) {
    return s1[2] < s2[2];
}