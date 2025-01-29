#include "nanobench.h"
#include "SkipList/SkipList.h"

#include <fstream>
#include <iostream>
#include <random>

//benchmarking insert algorithms
void bench(ankerl::nanobench::Bench* bench, SkipList* slist, int item) {
    bench->run(std::to_string(item), [&]() {
        slist->insert(item);
    });
}

int main() {
    // double d = 1.0;
    // ankerl::nanobench::Bench().run("Skip listing", [&] {
    //     SkipList list = SkipList();
    //     for(int i = 30; i > 0; i--){
    //         list.insert(i);
    //     }
    //     list.erase(21);
    //     list.search(21);
    // });

    ankerl::nanobench::Bench b;
    b.title("Skip List inserts")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(111);
    b.performanceCounters(true);
    SkipList slist = SkipList();
    for(int i = 0; i < 3000; i++){
        bench(&b, &slist, i);
    }
}