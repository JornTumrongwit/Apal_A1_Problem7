g++ -O3 -I../include nanobench.o SkipList.o test_erase.cpp -o erase_bench
$env:NANOBENCH_SUPPRESS_WARNINGS=0
.\erase_bench.exe