g++ -O3 -I../include SkipList.o test_insert_batch.cpp -o insert_batch_bench
$env:NANOBENCH_SUPPRESS_WARNINGS=0
.\insert_batch_bench.exe
