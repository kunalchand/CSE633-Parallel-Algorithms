#!/bin/bash

clear

# Clear the output file
rm output.txt

# Loop over the range of input values
for np in {1..245}
do
    # Print the current input value
    echo "Running script with $np / 3 processes"

    # Execute the original script with the current input value
    output=$(./run.sh <<<"$np" | tail -n 1)

    # Store the output in a file
    echo "$output" >> output.txt

    clear
done

echo "Execution Completed."
