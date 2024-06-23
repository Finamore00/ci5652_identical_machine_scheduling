#!/bin/bash

mkdir -p results/memetic_results6
mkdir -p results/memetic_results6/n=20
mkdir -p results/memetic_results6/n=25

export -f myfunc

myfunc() {
    n=$1
    m=$2
    echo "Benchmark Started. m=$m, n=$n"
    target/PROY3 benchmarks/n="$n"/m="$m" 1 > results/memetic_results6/n="$n"/"$n"-"$m"results_memetic6.txt
}

export -f myfunc

parallel myfunc ::: 20 25 ::: {2..10}