#!/bin/bash

# ls heuristic
echo "n = 20" 
echo "Procesando archivos de resultados de ls heuristic"
for file in ./n=20/*results_lsheuristic.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttLSH$filename.txt
        ./timels.sh $file > timeLSH$filename.txt
    done

# ls random
echo "Procesando archivos de resultados de ls random"
for file in ./n=20/*results_lsrandom.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttLSrandom$filename.txt
        ./timels.sh $file > timeLSrandom$filename.txt
    done

# heuristic
echo "Procesando archivos de resultados de heuristic"
for file in ./n=20/*hresults.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttH$filename.txt
        ./timeh.sh $file > timeH$filename.txt
    done

echo "/n"
echo "n = 25" 
echo "Procesando archivos de resultados de ls heuristic"
for file in ./n=25/*results_lsheuristic.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttLSH$filename.txt
        ./timels.sh $file > timeLSH$filename.txt
    done

# ls random
echo "Procesando archivos de resultados de ls random"
for file in ./n=25/*results_lsrandom.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttLSrandom$filename.txt
        ./timels.sh $file > timeLSrandom$filename.txt
    done

# heuristic
echo "Procesando archivos de resultados de heuristic"
for file in ./n=25/*hresults.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttH$filename.txt
        ./timeh.sh $file > timeH$filename.txt
    done