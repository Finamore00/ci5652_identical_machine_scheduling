#!/bin/bash

# Genetic algorithm
echo "Procesando archivos de resultados genetic algorithm"
echo "n=20"
for file in ./genetic_results/n=20/*results_genetic.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttGA$filename.txt
        ./timeGA.sh $file > timeGA$filename.txt
    done

echo "/n"
echo "n=25"
for file in ./genetic_results/n=25/*results_genetic.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttGA$filename.txt
        ./timeGA.sh $file > timeGA$filename.txt
    done


# Simulated annealing prueba 1
echo "Procesando archivos de resultados simulated annealing"
echo "n=20"
for file in ./simulated_annealing_results_1/n=20/*results_simulated_annealing.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttSA$filename.txt
        ./timeSA.sh $file > timeSA$filename.txt
    done

echo "/n"
echo "n=25"
for file in ./simulated_annealing_results_1/n=25/*results_simulated_annealing.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttSA$filename.txt
        ./timeSA.sh $file > timeSA$filename.txt
    done

