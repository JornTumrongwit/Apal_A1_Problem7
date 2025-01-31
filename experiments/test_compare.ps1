g++ -O3 -I../include nanobench.o SkipList.o test_compare.cpp -o compare_bench
$env:NANOBENCH_SUPPRESS_WARNINGS=0
.\compare_bench.exe