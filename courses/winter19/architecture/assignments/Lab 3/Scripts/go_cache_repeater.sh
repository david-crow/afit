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
CSIZES='16 64'
BSIZES='8 16 32'
ASSOCS='1 2 4'

for CSIZE in $CSIZES
do
    for BSIZE in $BSIZES
    do
        for ASSOC in $ASSOCS
        do
            echo "----------------------------------------"
            echo
            ./go_cache.sh $CSIZE $BSIZE $ASSOC
        done
    done
done

# footer
echo "----------------------------------------"
echo
echo "Total elapsed time: $(($SECONDS - START)) seconds"
echo
