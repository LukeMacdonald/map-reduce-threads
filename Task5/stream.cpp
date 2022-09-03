#include <random>
#include "stream.h"
Stream::Stream(const std::string& filename){
    std::ifstream file (filename);
    std::string word;
    length = 0;
    while(getline(file,word)){
        words.push_back(word);
        length++;
    }
    file.close();
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(words), std::end(words), rng);
}
std::string Stream::get(){
    std::string word = words[0];
    words.erase(words.begin());
    length--;
    return word;
    
}
int Stream::size(){
    return length;
}
