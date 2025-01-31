#include "nanobench.h"
#include "SkipList/SkipList.h"

#include <fstream>
#include <iostream>
#include <random>

//benchmarking insert algorithms
void bench(ankerl::nanobench::Bench* bench, SkipList* slist, int item, int index, std::string list_test) {
    bench->run(list_test + std::to_string(index) + " " + std::to_string(item), [&]() {
        slist->insert(item);
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

    ankerl::nanobench::Bench b_highp;
    b_highp.title("Skip List high prob")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(1);
    b_highp.performanceCounters(true);
    SkipList slist_hp = SkipList();
    slist_hp.p_set(0.8);
    for(int i = 0; i < 1000; i++){
        bench(&b_highp, &slist_hp, rand(), i, "high probability ");
    }

    gen("slist_hp.csv", ankerl::nanobench::templates::csv(), b_highp);

    ankerl::nanobench::Bench b_lowp;
    b_lowp.title("Skip List low prob")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(1);
    b_lowp.performanceCounters(true);
    SkipList slist_lp = SkipList();
    slist_lp.p_set(0.3);
    for(int i = 0; i < 1000; i++){
        bench(&b_lowp, &slist_lp, rand(), i, "low probability ");
    }

    gen("slist_lp.csv", ankerl::nanobench::templates::csv(), b_lowp);

    ankerl::nanobench::Bench b_highcap;
    b_highcap.title("Skip List high cap")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(1);
    b_highcap.performanceCounters(true);
    SkipList slist_hc = SkipList();
    slist_hc.cap = 15;
    for(int i = 0; i < 1000; i++){
        bench(&b_highcap, &slist_hc, rand(), i, "high level cap ");
    }

    gen("slist_hc.csv", ankerl::nanobench::templates::csv(), b_highcap);

    ankerl::nanobench::Bench b_lowcap;
    b_lowcap.title("Skip List low cap")
        .unit("uint64_t")
        .warmup(100)
        .relative(true)
        .minEpochIterations(1);
    b_lowcap.performanceCounters(true);
    SkipList slist_lc = SkipList();
    slist_lc.cap = 3;
    for(int i = 0; i < 1000; i++){
        bench(&b_lowcap, &slist_lc, rand(), i, "low level cap ");
    }

    gen("slist_lc.csv", ankerl::nanobench::templates::csv(), b_lowcap);
}