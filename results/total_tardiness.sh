#!/bin/bash

# Leer el archivo línea por línea
while IFS= read -r line
do
    # Buscar la línea que contiene "Total Tardiness"
    if [[ $line == *"Total Tardiness"* ]]; then
        echo $line | cut -d' ' -f3
    fi

done < $1