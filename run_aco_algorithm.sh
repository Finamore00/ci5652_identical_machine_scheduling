#!/bin/bash

mkdir -p results/aco_results4
mkdir -p results/aco_results4/n=20
mkdir -p results/aco_results4/n=25

myfunc() {
    n=$1
    m=$2
    echo "Benchmark Started. m=$m, n=$n"
    target/PROY3 benchmarks/n="$n"/m="$m" 3 > results/aco_results4/n="$n"/"$n"-"$m"results_aco4.txt
}

export -f myfunc

parallel myfunc ::: 20 25 ::: {2..10}