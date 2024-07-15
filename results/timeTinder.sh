#!/bin/bash

# Leer el archivo.txt
while IFS= read -r line
do
    # Buscar la línea que contiene "Time taken by tinder search"
    if [[ $line == *"Time taken by tinder search"* ]]; then
        # Extraer el valor de tiempo
        time=$(echo $line | awk '{print $6}' | sed 's/\./,/')
        echo "$time"
    fi
done < $1