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
INTALUS='1 2 3 4'
INTMULTS='2 3 4'
FPALUS='2 3 4'
FPMULTS='2 3 4'

for INTALU in $INTALUS
do
    echo "----------------------------------------"
    echo
    ./go.sh 1 $INTALU 1 1 1 1 taken out-of-order
done

for INTMULT in $INTMULTS
do
    echo "----------------------------------------"
    echo
    ./go.sh 1 1 $INTMULT 1 1 1 taken out-of-order
done

for FPALU in $FPALUS
do
    echo "----------------------------------------"
    echo
    ./go.sh 1 1 1 1 $FPALU 1 taken out-of-order
done

for FPMULT in $FPMULTS
do
    echo "----------------------------------------"
    echo
    ./go.sh 1 1 1 1 1 $FPMULT taken out-of-order
done

# footer
echo "----------------------------------------"
echo
echo "Total elapsed time: $(($SECONDS - START)) seconds"
echo
