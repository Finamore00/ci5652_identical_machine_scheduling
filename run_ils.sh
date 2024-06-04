#!/bin/bash

myfunc() {
    n=$1
    m=$2
    echo "Benchmark Started. m=$m, n=$n"
    target/PROY2 benchmarks/n="$n"/m="$m" 1 > results/ils_results/n="$n"/"$n"-"$m"results_ils.txt
}

export -f myfunc

parallel myfunc ::: 20 25 ::: {2..10}