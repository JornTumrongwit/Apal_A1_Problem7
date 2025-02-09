g++ -O3 -I../include SkipList.o test_search_batch.cpp -o search_batch_bench
$env:NANOBENCH_SUPPRESS_WARNINGS=0
.\search_batch_bench.exe