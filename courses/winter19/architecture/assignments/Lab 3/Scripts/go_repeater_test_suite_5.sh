#!/bin/bash

# header
echo
echo "CSCE 692 Lab 3"
date
echo
echo "Benchmark: go"
echo "Task: Microarchitecture Experimentation"
echo

# start the timer
START=$SECONDS

# establish the inputs
WIDTHS='1 2 4'
MEMPORTS='1 2 3 4'

for WIDTH in $WIDTHS
do
    for MEMPORT in $MEMPORTS
    do
        echo "----------------------------------------"
        echo
        ./go.sh $WIDTH 1 1 $MEMPORT 1 1 taken in-order
    done
done

# footer
echo "----------------------------------------"
echo
echo "Total elapsed time: $(($SECONDS - START)) seconds"
echo
