#!/bin/bash

myfunc() {
    n=$1
    m=$2
    echo "Benchmark Started. m=$m, n=$n"
    # run the grasp algorithm
    target/PROY2 benchmarks/n="$n"/m="$m" 4 > results/n="$n"/"$n"-"$m"GRASPresults.txt
}

run() {
    ns=(20 25)
    for n in "${ns[@]}"
    do
        for m in {2..10}
        do
            myfunc $n $m
        done
    done
}

run