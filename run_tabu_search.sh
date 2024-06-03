#!/bin/bash

myfunc() {
    n=$1
    m=$2
    echo "Benchmark Started. m=$m, n=$n"
    target/PROY2 benchmarks/n="$n"/m="$m" 2 > results/tabu_search_results/n="$n"/"$n"-"$m"results_tabu_search.txt
}

export -f myfunc

parallel myfunc ::: 20 25 ::: {2..10}