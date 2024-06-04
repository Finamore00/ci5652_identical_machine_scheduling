#!/bin/bash

# # Genetic algorithm prueba 1
# echo "Procesando archivos de resultados genetic algorithm"
# echo "n=20"
# for file in ./genetic_results/n=20/*results_genetic.txt
#     do
#         filename="${file##*/}"
#         filename="${filename%.*}"
#         echo "Procesando archivo: $filename"
#         ./total_tardiness.sh $file > ttGA$filename.txt
#         ./timeGA.sh $file > timeGA$filename.txt
#     done

# echo "/n"
# echo "n=25"
# for file in ./genetic_results/n=25/*results_genetic.txt
#     do
#         filename="${file##*/}"
#         filename="${filename%.*}"
#         echo "Procesando archivo: $filename"
#         ./total_tardiness.sh $file > ttGA$filename.txt
#         ./timeGA.sh $file > timeGA$filename.txt
#     done

# Genetic algorithm prueba 2
echo "Procesando archivos de resultados genetic algorithm"
echo "n=20"
for file in ./genetic_results_2/n=20/*results_genetic.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttGA2$filename.txt
        ./timeGA.sh $file > timeGA2$filename.txt
    done
echo "/n"
echo "n=25"
for file in ./genetic_results_2/n=25/*results_genetic.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttGA2$filename.txt
        ./timeGA.sh $file > timeGA2$filename.txt
    done

# Genetic algorithm prueba 3
echo "Procesando archivos de resultados genetic algorithm"
echo "n=20"
for file in ./genetic_results_3/n=20/*results_genetic.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttGA3$filename.txt
        ./timeGA.sh $file > timeGA3$filename.txt
    done
echo "/n"
echo "n=25"
for file in ./genetic_results_2/n=25/*results_genetic.txt
    do
        filename="${file##*/}"
        filename="${filename%.*}"
        echo "Procesando archivo: $filename"
        ./total_tardiness.sh $file > ttGA3$filename.txt
        ./timeGA.sh $file > timeGA3$filename.txt
    done

# # Simulated annealing prueba 1
# echo "Procesando archivos de resultados simulated annealing"
# echo "n=20"
# for file in ./simulated_annealing_results_1/n=20/*results_simulated_annealing.txt
#     do
#         filename="${file##*/}"
#         filename="${filename%.*}"
#         echo "Procesando archivo: $filename"
#         ./total_tardiness.sh $file > ttSA1$filename.txt
#         ./timeSA.sh $file > timeSA1$filename.txt
#     done

# echo "/n"
# echo "n=25"
# for file in ./simulated_annealing_results_1/n=25/*results_simulated_annealing.txt
#     do
#         filename="${file##*/}"
#         filename="${filename%.*}"
#         echo "Procesando archivo: $filename"
#         ./total_tardiness.sh $file > ttSA1$filename.txt
#         ./timeSA.sh $file > timeSA1$filename.txt
#     done

# # Simulated annealing prueba 2
# echo "Procesando archivos de resultados simulated annealing"
# echo "n=20"
# for file in ./simulated_annealing_results_2/n=20/*results_simulated_annealing.txt
#     do
#         filename="${file##*/}"
#         filename="${filename%.*}"
#         echo "Procesando archivo: $filename"
#         ./total_tardiness.sh $file > ttSA2$filename.txt
#         ./timeSA.sh $file > timeSA2$filename.txt
#     done

# echo "/n"
# echo "n=25"
# for file in ./simulated_annealing_results_2/n=25/*results_simulated_annealing.txt
#     do
#         filename="${file##*/}"
#         filename="${filename%.*}"
#         echo "Procesando archivo: $filename"
#         ./total_tardiness.sh $file > ttSA2$filename.txt
#         ./timeSA.sh $file > timeSA2$filename.txt
#     done


# # Simulated annealing prueba 3
# echo "Procesando archivos de resultados simulated annealing"
# echo "n=20"
# for file in ./simulated_annealing_results_3/n=20/*results_simulated_annealing.txt
#     do
#         filename="${file##*/}"
#         filename="${filename%.*}"
#         echo "Procesando archivo: $filename"
#         ./total_tardiness.sh $file > ttSA3$filename.txt
#         ./timeSA.sh $file > timeSA3$filename.txt
#     done

# echo "/n"
# echo "n=25"
# for file in ./simulated_annealing_results_3/n=25/*results_simulated_annealing.txt
#     do
#         filename="${file##*/}"
#         filename="${filename%.*}"
#         echo "Procesando archivo: $filename"
#         ./total_tardiness.sh $file > ttSA3$filename.txt
#         ./timeSA.sh $file > timeSA3$filename.txt
#     done
