#include "task1.h"
void task1filter(const std::string &read_file, const std::string &write_file){
    std::ifstream in_file;
    std::set<std::string> wordset;
    in_file.open(read_file);
    std::string output;
    while (getline(in_file, output)) {
        bool valid = true;
        auto size = (int)output.size();

        for (char i: output) {
            if (!isalpha(i)) {
                valid = false;
            }
        }
        if (size >= 3 && size <= 15 && valid) {
            wordset.insert(output);
        }
    }
    in_file.close();
    std::ofstream out_file;
    out_file.open(write_file);
    for (auto const &word: wordset) {
        out_file << word << std::endl;
    }
}