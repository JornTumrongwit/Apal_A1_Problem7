g++ -O3 -I../include SkipList.o test_erase_batch.cpp -o erase_batch_bench
$env:NANOBENCH_SUPPRESS_WARNINGS=0
.\erase_batch_bench.exe