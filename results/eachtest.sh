#!/bin/bash

# memetic algorithm prueba 1
echo "Procesando archivos de resultados memetic algorithm"
echo "n=20"
for file in ./memetic_results/n=20/*results_memetic.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttMA$filename.txt
        ./timeMA.sh $file > timeMA$filename.txt
    done

echo "/n"
echo "n=25"
for file in ./memetic_results/n=25/*results_memetic.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttMA$filename.txt
        ./timeMA.sh $file > timeMA$filename.txt
    done

# memetic algorithm prueba 2
echo "Procesando archivos de resultados memetic algorithm 2"
echo "n=20"
for file in ./memetic_results2/n=20/*results_memetic2.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttMA2$filename.txt
        ./timeMA.sh $file > timeMA2$filename.txt
    done

echo "/n"
echo "n=25"
for file in ./memetic_results2/n=25/*results_memetic2.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttMA2$filename.txt
        ./timeMA.sh $file > timeMA2$filename.txt
    done
