#!/bin/bash

export -f myfunc

myfunc() {
    n=$1
    m=$2
    echo "Benchmark Started. m=$m, n=$n"
    target/LocalSearch benchmarks/n="$n"/m="$m" 2 > results/n="$n"/"$n"-"$m"results_lsheuristic.txt
}

export -f myfunc

parallel myfunc ::: 20 25 ::: {2..10}