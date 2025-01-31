#include "nanobench.h"
#include "../SkipList/SkipList.h"

#include <fstream>
#include <iostream>
#include <random>
#include <map>
#include <set>
#include <unordered_set>

//benchmarking insert algorithms
void bench_insert_slist(ankerl::nanobench::Bench* bench, SkipList* slist, int item, int index, std::string list_test) {
    bench->run(list_test + std::to_string(index) + " " + std::to_string(item), [&]() {
        slist->insert(item);
    });
}

void bench_search_slist(ankerl::nanobench::Bench* bench, SkipList* slist, int item, int index, std::string list_test) {
    bench->run(list_test + std::to_string(index) + " " + std::to_string(item), [&]() {
        slist->search(item);
    });
}

void bench_erase_slist(ankerl::nanobench::Bench* bench, SkipList* slist, int item, int index, std::string list_test) {
    bench->run(list_test + std::to_string(index) + " " + std::to_string(item), [&]() {
        slist->erase(item);
    });
}

void bench_insert_map(ankerl::nanobench::Bench* bench, std::set<int>* item_map, int item, int index, std::string list_test) {
    bench->run(list_test + std::to_string(index) + " " + std::to_string(item), [&]() {
        ankerl::nanobench::doNotOptimizeAway(item_map->insert(item));
    });
}

void bench_search_map(ankerl::nanobench::Bench* bench, std::set<int>* item_map, int item, int index, std::string list_test) {
    bench->run(list_test + std::to_string(index) + " " + std::to_string(item), [&]() {
        ankerl::nanobench::doNotOptimizeAway(item_map->count(item));
    });
}

void bench_erase_map(ankerl::nanobench::Bench* bench, std::set<int>* item_map, int item, int index, std::string list_test) {
    bench->run(list_test + std::to_string(index) + " " + std::to_string(item), [&]() {
        ankerl::nanobench::doNotOptimizeAway(item_map->erase(item));
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

    SkipList slist = SkipList();
    std::map<int, int> i_map; //This should be the map
    std::set<int> items;
    std::unordered_set<int> u_items;

    ankerl::nanobench::Bench b_slist_insert;
    b_slist_insert.title("Skip List high prob")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(1);
    b_slist_insert.performanceCounters(true);

    ankerl::nanobench::Bench b_map_insert;
    b_map_insert.title("Skip List high prob")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(1);
    b_map_insert.performanceCounters(true);

    ankerl::nanobench::Bench b_slist_search;
    b_slist_search.title("Skip List high prob")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(1);
    b_slist_search.performanceCounters(true);

    ankerl::nanobench::Bench b_map_search;
    b_map_search.title("Skip List high prob")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(1);
    b_map_search.performanceCounters(true);

    ankerl::nanobench::Bench b_slist_erase;
    b_slist_erase.title("Skip List high prob")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(1);
    b_slist_erase.performanceCounters(true);

    ankerl::nanobench::Bench b_map_erase;
    b_map_erase.title("Skip List high prob")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(1);
    b_map_erase.performanceCounters(true);
    
    items.clear();
    slist = SkipList();
    for(int i = 0; i < 3000; i++){
        int item = rand();
        while(items.count(item)) item = rand();

        items.insert(item);
        slist.insert(item);
        u_items.insert(item);
    }

    int i=0;
    for(int item: u_items){
        bench_erase_slist(&b_slist_erase, &slist, item, i, "slist erase ");
        bench_erase_map(&b_map_erase, &items, item, i, "map erase ");
        i++;
    }

    items.clear();
    slist = SkipList();
    for(int i = 0; i < 3000; i++){
        int item = rand();
        while(items.count(item)) item = rand();
        items.insert(item);

        slist.insert(item);
        i_map.insert({item, item});

        int search_item = rand();
        bench_search_slist(&b_slist_search, &slist, item, i, "slist search ");
        bench_search_map(&b_map_search, &items, item, i, "map search ");
    }
    
    items.clear();
    slist = SkipList();
    i_map.clear();
    for(int i = 0; i < 3000; i++){
        int item = rand();
        while(items.count(item)) item = rand();
        bench_insert_slist(&b_slist_insert, &slist, item, i, "slist insert ");
        bench_insert_map(&b_map_insert, &items, item, i, "map insert ");
    }

    

    gen("compare_map_erase.csv", ankerl::nanobench::templates::csv(), b_map_erase);
    gen("compare_slist_erase.csv", ankerl::nanobench::templates::csv(), b_slist_erase);
    gen("compare_map_search.csv", ankerl::nanobench::templates::csv(), b_map_search);
    gen("compare_slist_search.csv", ankerl::nanobench::templates::csv(), b_slist_search);
    gen("compare_map_insert.csv", ankerl::nanobench::templates::csv(), b_map_insert);
    gen("compare_slist_insert.csv", ankerl::nanobench::templates::csv(), b_slist_insert);
}