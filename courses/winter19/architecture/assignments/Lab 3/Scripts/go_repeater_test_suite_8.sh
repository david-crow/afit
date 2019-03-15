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
BPREDS='taken nottaken 2lev'

for WIDTH in $WIDTHS
do
    for BPRED in $BPREDS
    do
        echo "----------------------------------------"
        echo
        ./go.sh $WIDTH 1 1 1 1 1 $BPRED in-order
    done
done

# footer
echo "----------------------------------------"
echo
echo "Total elapsed time: $(($SECONDS - START)) seconds"
echo
