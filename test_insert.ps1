g++ -O3 -I../include nanobench.o SkipList.o test_insert.cpp -o insert_bench
$env:NANOBENCH_SUPPRESS_WARNINGS=0
.\insert_bench.exe