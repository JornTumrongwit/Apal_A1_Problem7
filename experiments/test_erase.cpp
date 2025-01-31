#include "nanobench.h"
#include "../SkipList/SkipList.h"

#include <fstream>
#include <iostream>
#include <random>
#include <set>

//benchmarking insert algorithms
void bench(ankerl::nanobench::Bench* bench, SkipList* slist, int index, int item, std::string list_test) {
    bench->run(list_test + std::to_string(index), [&]() {
        slist->erase(item);
    });
}

//rendering out
void gen(std::string const& fname, char const* mustacheTemplate,
         ankerl::nanobench::Bench const& bench) {

    std::ofstream templateOut(fname);
    templateOut << mustacheTemplate;

    std::ofstream renderOut(fname);
    ankerl::nanobench::render(mustacheTemplate, bench, renderOut);
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

    ankerl::nanobench::Bench b_erase;
    b_erase.title("Skip List erases")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(1);
    b_erase.performanceCounters(true);

    SkipList slist = SkipList();

    std::set<int> items;
    for(int i = 0; i < 3000; i++){
        int item = rand();
        while(items.count(item)) item = rand();
        items.insert(item);
        slist.insert(item);
    }

    int i = 0;
    for(int item: items){
        bench(&b_erase, &slist, i, item, "delete ");
        i++;
    }

    gen("slist_deletes.csv", ankerl::nanobench::templates::csv(), b_erase);
}