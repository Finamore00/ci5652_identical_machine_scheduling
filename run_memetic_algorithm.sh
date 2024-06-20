#!/bin/bash

mkdir -p results/memetic_results3
mkdir -p results/memetic_results3/n=20
mkdir -p results/memetic_results3/n=25

export -f myfunc

myfunc() {
    n=$1
    m=$2
    echo "Benchmark Started. m=$m, n=$n"
    target/PROY3 benchmarks/n="$n"/m="$m" 1 > results/memetic_results3/n="$n"/"$n"-"$m"results_memetic3.txt
}

export -f myfunc

parallel myfunc ::: 25 ::: {7..10}