g++ -O3 -I../include nanobench.o SkipList.o test_search.cpp -o search_bench
$env:NANOBENCH_SUPPRESS_WARNINGS=0
.\search_bench.exe