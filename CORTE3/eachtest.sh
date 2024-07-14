#!/bin/bash

# memetic algorithm prueba 4
echo "Procesando archivos de resultados scattered search 6"
echo "n=20"
for file in ./scattered_results6/n=20/*results_scattered6.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttSS$filename.txt
        ./timeScatter.sh $file > timeSS$filename.txt
    done

echo "/n"
echo "n=25"
for file in ./scattered_results6/n=25/*results_scattered6.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttSS$filename.txt
        ./timeScatter.sh $file > timeSS$filename.txt
    done
