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

# memetic algorithm prueba 4
echo "Procesando archivos de resultados memetic algorithm 4"
echo "n=20"
for file in ./memetic_results4/n=20/*results_memetic4.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttMA4$filename.txt
        ./timeMA.sh $file > timeMA4$filename.txt
    done

echo "/n"
echo "n=25"
for file in ./memetic_results4/n=25/*results_memetic4.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttMA4$filename.txt
        ./timeMA.sh $file > timeMA4$filename.txt
    done
