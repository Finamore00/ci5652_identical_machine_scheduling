#!/bin/bash

# Leer el archivo.txt
while IFS= read -r line
do
    # Buscar la línea que contiene "Time taken by memetic algorithm"
    if [[ $line == *"Time taken by scattered_search"* ]]; then
        # Extraer el valor de tiempo
        time=$(echo $line | awk '{print $5}' | sed 's/\./,/')
        echo "$time"
    fi
done < $1