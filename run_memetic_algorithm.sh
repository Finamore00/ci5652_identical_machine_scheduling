#!/bin/bash

mkdir -p results/memetic_results5
mkdir -p results/memetic_results5/n=20
mkdir -p results/memetic_results5/n=25

export -f myfunc

for i in {2..10}
do
    echo "Benchmark Started. m=$i, n=25"
    target/PROY3 benchmarks/n=25/m=$i 1 > results/memetic_results5/n=25/25-$i-results_memetic5.txt
done