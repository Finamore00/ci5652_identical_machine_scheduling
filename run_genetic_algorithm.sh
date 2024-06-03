#!/bin/bash

mkdir -p results/genetic_results2
mkdir -p results/genetic_results2/n=20
mkdir -p results/genetic_results2/n=25

export -f myfunc

myfunc() {
    n=$1
    m=$2
    echo "Benchmark Started. m=$m, n=$n"
    target/PROY2 benchmarks/n="$n"/m="$m" 5 > results/genetic_results2/n="$n"/"$n"-"$m"results_genetic.txt
}

export -f myfunc

parallel myfunc ::: 20 25 ::: {2..10}