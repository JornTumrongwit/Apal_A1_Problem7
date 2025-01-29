#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"
#include "SkipList/SkipList.h"

int main() {
    double d = 1.0;
    ankerl::nanobench::Bench().run("Skip listing", [&] {
        SkipList list = SkipList();
        for(int i = 30; i > 0; i--){
            list.insert(i);
        }
        list.erase(21);
        list.search(21);
    });
}