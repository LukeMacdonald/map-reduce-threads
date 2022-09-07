#include "Task1Filter.h"
#include <random>

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
        if (size > 2 && size < 16){
            // Checks that all characters in the word are alphabetic
            for (char c: line){
                if (!isalpha(c)) {
                    valid = false;
                }
            }
            // If word length is between 3-15 and only contains
            // alphabetic characters the word is added to the set.
            if(valid){
                // Adding word to vector
                wordset.push_back(line);
            }
        }
    }
    // Closes dirty file after all lines have been read over;
    input_file.close();
    
    // Function to remove duplicate within the specified range
    // The specified range here is the beginning and end of vector
    std::unique(wordset.begin(),wordset.end());

    // algorithm that generates random numbers using computers internal clock 
    std::random_device engine;
    // function to shuffle the vector to get it in random order
    std::shuffle(wordset.begin(), wordset.end(),engine);

    // Opens file to output filtered words
    std::ofstream out_file(write_file);
    
    // Writes all words in vector to file
    for (std::string word: wordset) {
        out_file << word << std::endl;
    }
}