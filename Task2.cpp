#include "Task2.h"
int main(int argc, char *argv[]){
     // First command line argument is the name of dirty file
    std::string input_file = argv[1];
    // Second command line argument is the name of the clean file
    std::string output_file= argv[2];
    // Function to filter to dirty file into the clean file.
    task1Filter(input_file,output_file);
    map2(output_file);
    for(int i = 0; i < PROCESS_NUM;i++){
        std::string file = "File" + std::to_string(i + 3) + ".txt";
        remove(file.c_str());
    }
    
    return EXIT_SUCCESS;
}
void map2(std::string filename){
    
    // Declares and opens file to map
    std::ifstream input_file(filename);
    // Declares an array of vectors. One vector for 
    // each length of word from 3 to 15
    std::vector<std::string> index[PROCESS_NUM];
    // Declares string to store current line of file
    // input
    std::string line;
    std::cout << "Reading Words in Vectors From File:" << filename << "\n";
    // While loop to read through entire file
    while (std::getline(input_file, line)) {
        // gets the length of the current word being read
        auto size = int(line.size());
        // stores that word into the vector that correlates to
        // the size of the string. E.g. of length 3 will be stored
        // in index[0] vector.
        index[size - 3].push_back(line);
    }
    std::cout << "Reading Words From File:" << filename << " And Sorting Those Words into" 
                <<" Length Vectors Now Completed\n";
    // Closes cleaned input file
    input_file.close();
    int index_size;
    for (int i = 0; i < PROCESS_NUM; i++) {
        // An if statement which forks to create a new process
        // and if the process is equal to 0, i.e. child process
        // then the child function is called.
        if (fork()== 0) {
            index_size = i + 3;
            std::cout << "CHILD PROCESS: For Length Vector " << index_size << " Now Starting!\n";
            // Calls function to map vector to correlating index file
            child_function(index_size,index[i]);
            std::cout << "CHILD PROCESS: For Length Vector " << index_size << " Has Completed!\n";
            // Call to exit the child process
            exit(EXIT_SUCCESS);
        }
    }
    // For loop to calls to wait until all child processes have completed
    for (int i = 0; i <= PROCESS_NUM; i++) {
        wait(nullptr);
    }
    std::cout << "MAP2 HAS COMPLETED!!!"<<std::endl;
    // Function call to reduce 
    std::cout << "PARENT PROCESS: Reduce Method Now Starting!\n";
    reduce2(filename);
    
}
void reduce2(std::string filename){
    
    std::ifstream files[13];
    std::string length_file;
    std::string smallest;
    std::vector<std::string> all_words;

    // Loop for opening and getting first line of each files
    for (int i = 0; i < PROCESS_NUM; i++){
        length_file = "File" + std::to_string(i + 3) + ".txt";
        files[i].open(length_file);
        std::cout << "OPENING FILE: " << length_file << "\n";
        if (std::getline(files[i], smallest)){
            all_words.push_back(smallest);
        }
    }
    
    std::cout << "Now Sorting And Merging List To File: " << filename << "\n";
    std::ofstream output(filename);
   
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
        if(getline(files[index],smallest)){
            all_words.push_back(smallest);
        }
    }
    // Closes file that reduce is writing to.
    output.close();
    std::cout << "Merge And Sort Now Completed!\n";
    // Closes all the different word length files
    for (int i = 0; i < 13; i++){
        std::cout << "CLOSING FILE: Task2Files/File" << i + 3 << ".txt\n";
        files[i].close();
    }
    std::cout << "REDUCE2 HAS COMPLETED!!!"<<std::endl;
}
void child_function(int index,std::vector<std::string> list){
    std::ofstream file;
    // Sorts the vector of strings based on their third character onwards
    sort(list.begin(),list.end(), sort_string);
    // Opens file representing the string size that vector stores
    file.open("File" + std::to_string(index) + ".txt");
    // Writes all words to that file
    for (std::string &line: list) {
        file << line << std::endl;
    }
    file.close();
}