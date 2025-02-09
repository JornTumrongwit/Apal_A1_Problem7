#!/bin/bash
touch space_map.csv
echo size,mem_MB >> space_map.csv

for i in {0..9}
do
    ./insert_valgrinder.sh ${i}
done