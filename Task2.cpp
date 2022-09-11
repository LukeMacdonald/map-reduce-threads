#include "Task2.h"

OutputHandler output_handler;
int main(int argc, char *argv[]){
    clock_t start,end;
    start = clock();
    output_handler.open_files("Task2Files");
     // First command line argument is the name of dirty file
    std::string input_file = argv[1];
    // Second command line argument is the name of the clean file
    std::string output_file= argv[2];
    // Function to filter to dirty file into the clean file.
    std::string filter = "Task2Files/filter/filter.txt";
    task1Filter(input_file,filter);
    map2(output_file);
    std::cout << "Task 2 Finish Completed!\nOutput File is located in in 'Task2Files/output' directory as '" 
    << output_file <<  "'\n";
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_handler.print_exec_time("Total",time_taken);
    output_handler.close_files();
    return EXIT_SUCCESS;
}
void map2(std::string filename){
    clock_t start,end;
    start = clock();
    
    // Declares and opens file to map
    std::ifstream input_file("Task2Files/filter/filter.txt");
    // Declares an array of vectors. One vector for 
    // each length of word from 3 to 15
    std::vector<std::string> index[PROCESS_NUM];
    // Declares string to store current line of file
    // input
    std::string line;
    output_handler.print_log("Reading Words in Vectors From File:" + filename);
    // While loop to read through entire file
    while (std::getline(input_file, line)) {
        // gets the length of the current word being read
        auto size = int(line.size());
        // stores that word into the vector that correlates to
        // the size of the string. E.g. of length 3 will be stored
        // in index[0] vectooutput_handler.
        index[size - 3].push_back(line);
    }
    output_handler.print_log("Reading Words From File:" + filename + " And Sorting Those Words into" 
                + " Length Vectors Now Completed");
    // Closes cleaned input file
    input_file.close();
    int index_size;
    for (int i = 0; i < PROCESS_NUM; i++) {
        // An if statement which forks to create a new process
        // and if the process is equal to 0, i.e. child process
        // then the child function is called.
        if (fork()== 0) {
            clock_t start_process,end_process;
            start_process = clock();
            
            index_size = i + 3;
            output_handler.print_log("Map2 child process: For Length Vector " + std::to_string(index_size) + " Now Starting!");
            // Calls function to map vector to correlating index file
            child_function(index_size,index[i]);
            output_handler.print_log("Map2 child process: For Length Vector " + std::to_string(index_size) + " Has Completed!");
            // Call to exit the child process
             end_process = clock();
             double time_taken_process = double(end_process - start_process) / double(CLOCKS_PER_SEC);
             output_handler.print_exec_time("Map2 Process"+ std::to_string(index_size),time_taken_process);
            exit(EXIT_SUCCESS);
        }
    }
    // For loop to calls to wait until all child processes have completed
    for (int i = 0; i <= PROCESS_NUM; i++) {
        wait(nullptr);
    }
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_handler.print_exec_time("Map2",time_taken);
    output_handler.print_log("Map2 has completed!");
    // Function call to reduce 
    output_handler.print_log("Parent process: Reduce Method Now Starting!");
    reduce2(filename);
    
}
void reduce2(std::string filename){
    clock_t start,end;
    start = clock();
    
    std::ifstream files[13];
    std::string length_file;
    std::string smallest;
    std::vector<std::string> all_words;

    // Loop for opening and getting first line of each files
    for (int i = 0; i < PROCESS_NUM; i++){
        length_file = "Task2Files/MapFolder/File" + std::to_string(i + 3) + ".txt";
        files[i].open(length_file);
        output_handler.print_log("Opening: " + length_file);
        if (std::getline(files[i], smallest)){
            all_words.push_back(smallest);
        }
    }
    
    output_handler.print_log("Now Sorting And Merging List To File: " + filename);;
    std::string file = "Task2Files/output/" + filename;
    std::ofstream output(file);
   
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
    output_handler.print_log("Merge And Sort Now Completed!");
    // Closes all the different word length files
    for (int i = 0; i < 13; i++){
        output_handler.print_log("Closing Task2Files/MapFolder/File" + std::to_string(i + 3) + ".txt");
        files[i].close();
    }
    output_handler.print_log("Reduce2 has completed!");
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_handler.print_exec_time("Reduce2",time_taken);
}
void child_function(int index,std::vector<std::string> list){
    std::ofstream file;
    // Sorts the vector of strings based on their third character onwards
    sort(list.begin(),list.end(), sort_string);
    // Opens file representing the string size that vector stores
    file.open("Task2Files/MapFolder/File" + std::to_string(index) + ".txt");
    // Writes all words to that file
    for (std::string &line: list) {
        file << line << std::endl;
    }
    file.close();

}