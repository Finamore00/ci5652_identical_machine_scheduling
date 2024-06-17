#!/bin/bash

# Specify the directory path
directory=./

# Iterate over files in the directory
for filename in $(find "$directory" -type f); do
    # Extract the alpha number and the number of iterations
    alpha=$(echo "$filename" | cut -d '_' -f 2 | cut -d '.' -f 1,2)
    iterations=$(echo "$filename" | cut -d '_' -f 4 | cut -d '.' -f 1)

    # Perform your desired action with the extracted values
    if [[ "$alpha" == "0.5" ]]; then
        echo "Alpha: 0.5, Iterations: $iterations"
        ./../../../total_tardiness.sh $filename > tt0.5-$iterations.txt
        ./../../../timegrasp.sh $filename > time0.5-$iterations.txt
    elif [[ "$alpha" == "0.75" ]]; then
        echo "Alpha: 0.75, Iterations: $iterations"
        ./../../../total_tardiness.sh $filename > tt0.75-$iterations.txt
        ./../../../timegrasp.sh $filename > time0.75-$iterations.txt
    elif [[ "$alpha" == "0.25" ]]; then
        echo "Alpha: 0.25, Iterations: $iterations"
        ./../../../total_tardiness.sh $filename > tt0.25-$iterations.txt
        ./../../../timegrasp.sh $filename > time0.25-$iterations.txt
    fi

done