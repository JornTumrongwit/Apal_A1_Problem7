#!/bin/bash
order=$(($1*1000000+1000000))
echo ${order}
g++ -O3 -I../include SkipList.o test_mem.cpp -o mem_bench
valgrind --tool=massif --massif-out-file="massif.${order}" ./mem_bench ${order}

item=$(ms_print massif.${order} | grep -oE "[0-9]+.[0-9]+\^" |  tr -d '^')
echo ${order},${item} >> space_map.csv