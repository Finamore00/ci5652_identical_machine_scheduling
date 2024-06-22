#!/bin/bash

mkdir -p results/aco_results
mkdir -p results/aco_results/n=20
mkdir -p results/aco_results/n=25

myfunc() {
    n=$1
    m=$2
    echo "Benchmark Started. m=$m, n=$n"
    target/PROY3 benchmarks/n="$n"/m="$m" 3 > results/aco_results/n="$n"/"$n"-"$m"results_aco.txt
}

export -f myfunc

parallel myfunc ::: 20 25 ::: {2..10}