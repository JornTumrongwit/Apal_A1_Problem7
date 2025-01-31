#include "nanobench.h"
#include "../SkipList/SkipList.h"

#include <fstream>
#include <iostream>
#include <random>
#include <set>

//benchmarking insert algorithms
void bench(ankerl::nanobench::Bench* bench, SkipList* slist, int item, int index, std::string list_test) {
    bench->run(list_test + std::to_string(index) + " " + std::to_string(item), [&]() {
        slist->search(item);
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

    ankerl::nanobench::Bench b_miss;
    b_miss.title("Skip List misses")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(1);
    b_miss.performanceCounters(true);

    ankerl::nanobench::Bench b_hits;
    b_hits.title("Skip List hits")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(1);
    b_hits.performanceCounters(true);

    ankerl::nanobench::Bench b_random;
    b_random.title("Skip List random searches")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(1);
    b_random.performanceCounters(true);
    SkipList slist = SkipList();

    std::set<int> items;
    for(int i = 0; i < 3000; i++){
        int item = rand();
        while(items.count(item)) item = rand();
        items.insert(item);
        slist.insert(item);
        bench(&b_hits, &slist, item, i, "hits ");
        bench(&b_miss, &slist, -1, i, "miss ");
        bench(&b_random, &slist, -1, i, "random ");
    }

    gen("slist_miss_search.csv", ankerl::nanobench::templates::csv(), b_miss);
    gen("slist_hit_search.csv", ankerl::nanobench::templates::csv(), b_hits);
    gen("slist_random_search.csv", ankerl::nanobench::templates::csv(), b_random);
}