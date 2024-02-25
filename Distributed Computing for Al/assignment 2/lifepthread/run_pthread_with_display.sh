#!/bin/bash

# Compile the PThread version of the program
gcc -o life life.c plot.c real_rand.c timer.c -pthread

# Output file
output_file="results_pthread_with_display.txt"

# Clear the output file
> $output_file

# Define the array of input sizes
input_sizes=(200 2000 4000 6000 8000 10000)

# Define the array of thread counts
thread_counts=(1 2 4 8 16 32)

# Loop over each input size
for size in "${input_sizes[@]}"
do
    # Add a title for each new input size to the output file
    echo "========================= Input Size: $size =========================" >> $output_file

    # Loop over each thread count
    for threads in "${thread_counts[@]}"
    do
        # Add a title for each configuration to the output file
        echo "Running PThread with display - Size: $size, Threads: $threads" >> $output_file

        # Construct the command
        cmd="./life -s 1234 -n $size -i 500 -p 0.5 -t $threads -g 0"

        # Print the current configuration to the console and run the command
        echo "Running: $cmd"
        $cmd >> $output_file 2>&1

        # Add a separator for readability
        echo "---------------------------------------------------" >> $output_file
    done
done

echo "All PThread simulations with display completed."
