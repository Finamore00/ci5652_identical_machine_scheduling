#!/bin/bash

# Recorrer todas las carpetas que terminan en "graspresults"
for dir in */
do
    # Ir a la carpeta actual
    cd "$dir"
    
    # Ejecutar el archivo run.sh
    ./run.sh
    
    # Regresar a la carpeta anterior
    cd ..
done