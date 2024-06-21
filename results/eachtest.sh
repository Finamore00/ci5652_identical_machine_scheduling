#!/bin/bash

# memetic algorithm prueba 1
# echo "Procesando archivos de resultados memetic algorithm"
# echo "n=20"
# for file in ./memetic_results/n=20/*results_memetic.txt
#     do
#         filename="${file##*/}"
#         filename="${filename%.*}"
#         echo "Procesando archivo: $filename"
#         ./total_tardiness.sh $file > ttMA$filename.txt
#         ./timeMA.sh $file > timeMA$filename.txt
#     done

# echo "/n"
# echo "n=25"
# for file in ./memetic_results/n=25/*results_memetic.txt
#     do
#         filename="${file##*/}"
#         filename="${filename%.*}"
#         echo "Procesando archivo: $filename"
#         ./total_tardiness.sh $file > ttMA$filename.txt
#         ./timeMA.sh $file > timeMA$filename.txt
#     done

# memetic algorithm prueba 3
echo "Procesando archivos de resultados memetic algorithm 3"
echo "n=20"
for file in ./memetic_results3/n=20/*results_memetic3.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttMA3$filename.txt
        ./timeMA.sh $file > timeMA3$filename.txt
    done

echo "/n"
echo "n=25"
for file in ./memetic_results3/n=25/*results_memetic3.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttMA3$filename.txt
        ./timeMA.sh $file > timeMA3$filename.txt
    done
