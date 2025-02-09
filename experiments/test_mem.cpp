#include "../SkipList/SkipList.h"

#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <set>

//benchmarking insert algorithms
void bench(int index, std::string list_test) {
    SkipList list = SkipList<32>();
    
    for(int i = 0; i < index; i++){
        list.insert(std::hash<int>{}(i));
    }
    std::cout<< list.search(4235)<< "\n";
    return;
}

void benchmap(int index, std::string list_test) {
    std::set<int> list;
    for(int i = 0; i < index; i++){
        list.insert(std::hash<int>{}(i));
    }
    printf("%f\n", list.size()); //needed this so it won't just disappear when compiling
    return;
}

int main(int argc, char* argv[]) {
    benchmap(atoi(argv[1]), "List size ");
}