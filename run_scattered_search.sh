#!/bin/bash

export -f myfunc

myfunc() {
    n=$1
    m=$2
    echo "Benchmark Started. m=$m, n=$n"
    target/PROY3 benchmarks/n="$n"/m="$m" 2 > results/scattered_results6/n="$n"/"$n"-"$m"results_scattered6.txt
}

export -f myfunc

parallel myfunc ::: 20 25 ::: {2..10}