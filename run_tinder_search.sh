#!/bin/bash

mkdir -p results/tinder_search5
mkdir -p results/tinder_search5/n=20
mkdir -p results/tinder_search5/n=25

export -f myfunc

myfunc() {
    n=$1
    m=$2
    echo "Benchmark Started. m=$m, n=$n"
    target/TINDER_SEARCH benchmarks/n="$n"/m="$m" > results/tinder_search5/n="$n"/"$n"-"$m"results_tinderSearch5.txt
}

export -f myfunc

parallel myfunc ::: 20 25 ::: {2..10}