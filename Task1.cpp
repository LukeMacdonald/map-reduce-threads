#include "Task1.h"

OutputHandler output_handler;

int main(int argc, char *argv[]){
    output_handler.open_files("Task1Files");
    clock_t start, end;
    start = clock();
    // First command line argument is the name of dirty file
    std::string input_file = argv[1];
    // Second command line argument is the name of the clean file
    std::string output_file= argv[2];
    // Function to filter to dirty file into the clean file.
    task1Filter(input_file,"Task1Files/output/"+output_file);
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    output_handler.print_exec_time("Task1",time_taken);
    output_handler.print_log("Task 1 Filtering has completed!");
    output_handler.close_files();
    return EXIT_SUCCESS;

}
void task1Filter(std::string read_file, std::string write_file){
    
    // Declares and Opens Dirty File
    std::ifstream input_file(read_file);
    // Declares vector to add filtered words to\td
    std::vector<std::string> wordset;
    // Declares string used to store the word of each line
    std::string line;
    // While loops scans over each line in file
    while (getline(input_file, line)) {
        bool valid = true;
        int size = line.size();
        // Checks that size of word is greater than 2 and less 
        // than 16
        output_handler.print_log("Checking size of string is between 3 and 15");
        if (size > 2 && size < 16){
            // Checks that all characters in the word are alphabetic
            output_handler.print_log("Checking that string contains only alphabetic characters");
            for (char c: line){
                if (!isalpha(c)) {
                    valid = false;
                }
            }
            // If word length is between 3-15 and only contains
            // alphabetic characters the word is added to the set.
            
            if(valid){
                output_handler.print_log("Adding Valid Word Back to File: " + line);
                // Adding word to vector
                wordset.push_back(line);
            }
        }
    }
    // Closes dirty file after all lines have been read over;
    input_file.close();
    
    // Function to remove duplicate within the specified range
    // The specified range here is the beginning and end of vector
    output_handler.print_log("Checking for uniqueness of strings");
    std::unique(wordset.begin(),wordset.end());


    // Opens file to output filtered words
    std::ofstream out_file(write_file);
    output_handler.print_log("Writing all words out to output file");
    
    // Writes all words in vector to file
    for (std::string word: wordset) {
        out_file << word << std::endl;
    }
}