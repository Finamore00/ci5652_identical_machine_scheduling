#!/bin/bash

mkdir -p results/tinder_search3
mkdir -p results/tinder_search3/n=20
mkdir -p results/tinder_search3/n=25

export -f myfunc

myfunc() {
    n=$1
    m=$2
    echo "Benchmark Started. m=$m, n=$n"
    target/TINDER_SEARCH benchmarks/n="$n"/m="$m" > results/tinder_search3/n="$n"/"$n"-"$m"results_tinderSearch2.txt
}

export -f myfunc

parallel myfunc ::: 20 25 ::: {2..10}