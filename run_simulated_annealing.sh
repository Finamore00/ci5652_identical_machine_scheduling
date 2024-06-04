#!/bin/bash

myfunc() {
    n=$1
    m=$2
    echo "Benchmark Started. m=$m, n=$n"
    target/PROY2 benchmarks/n="$n"/m="$m" 3 > ./results/simulated_annealing_results_1/n="$n"/"$n"-"$m"results_simulated_annealing.txt
}

export -f myfunc

parallel myfunc ::: 20 25 ::: {2..10}