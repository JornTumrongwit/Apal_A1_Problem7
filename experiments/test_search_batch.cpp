#include "../SkipList/SkipList.h"

#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <set>

//benchmarking insert algorithms
long bench(int index, std::string list_test) {
    SkipList list = SkipList<32>();
    long t;
    
    for(int i = 0; i < index; i++){
        list.insert(std::hash<int>{}(i));
    }
    bool temp;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 100000; i++){
        temp = list.search(std::hash<int>{}(i));
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout<< temp<< "\n";
    t = std::chrono::duration_cast<std::chrono::microseconds>(finish-start).count();
    return t;
}

long bench_bidir(int index, std::string list_test) {
    SkipList list = SkipList<32>();
    long t;
    
    for(int i = 0; i < index; i++){
        list.insert(std::hash<int>{}(i));
    }
    bool temp;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 100000; i++){
        temp = list.bi_search(std::hash<int>{}(i));
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout<< temp<< "\n";
    t = std::chrono::duration_cast<std::chrono::microseconds>(finish-start).count();
    return t;
}

long benchmap(int index, std::string list_test) {
    std::set<int> list;
    long t;
    for(int i = 0; i < index; i++){
        list.insert(std::hash<int>{}(i));
    }
    std::set<int>::iterator temp;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 100000; i++){
        temp = list.find(std::hash<int>{}(i));
    }
    auto finish = std::chrono::high_resolution_clock::now();
    printf("%d\n", temp); //needed this so it won't just disappear when compiling
    t = std::chrono::duration_cast<std::chrono::microseconds>(finish-start).count();
    return t;
}

int main() {
    std::ofstream file;
    file.open("search_bidir.csv", std::ios_base::app);
    long t = 0;
    int epoch_amt = 1;
    file <<  "list_size,time\n";
    for(int i = 100000; i < 10000001; i+=100000){
        for(int epoch = 1; epoch <= epoch_amt; epoch++){
            long time = bench_bidir(i, "List size ");
            t += time;
        }
        file <<  i << "," << float(t)/float(epoch_amt) << "\n";
        std::cout <<  i << "," << float(t)/float(epoch_amt) << "\n";
        t = 0;
    }
    file.close();
}