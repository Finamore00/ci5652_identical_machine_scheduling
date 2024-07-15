#!/bin/bash

mkdir -p separate_results
mkdir -p separate_results/tinder_search
mkdir -p separate_results/tinder_search/n=20
mkdir -p separate_results/tinder_search/n=25

echo "Procesando archivos de resultados scattered search"
echo "n=20"
for file in ./tinder_search3/n=20/*results_tinderSearch2.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > separate_results/tinder_search/n=20/ttTinder$filename.txt
        ./timeTinder.sh $file > separate_results/tinder_search/n=20/timeTinder$filename.txt
    done

echo "/n"
echo "n=25"
for file in ./tinder_search3/n=25/*results_tinderSearch2.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > separate_results/tinder_search/n=25/ttTinder$filename.txt
        ./timeTinder.sh $file > separate_results/tinder_search/n=25/timeTinder$filename.txt
    done
