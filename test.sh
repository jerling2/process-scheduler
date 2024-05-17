#!/bin/bash

# Launch the program 10 times concurrently
touch output.log
for i in {1..100}; do
    ./part2 input.txt >> output.log 2>&1 &  # Replace 'your_program' with the name of your program
done

