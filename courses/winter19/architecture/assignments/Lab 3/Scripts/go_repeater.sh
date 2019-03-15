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
INTALUS='1 2 3 4'
INTMULTS='1 2 3 4'
MEMPORTS='1 2 3 4'
FPALUS='1 2 3 4'
FPMULTS='1 2 3 4'
BPREDS='taken nottaken 2lev'
ORDERS='in-order out-of-order'

# run all input combinations
for WIDTH in $WIDTHS
do
    for INTALU in $INTALUS
    do
        for INTMULT in $INTMULTS
        do
            for MEMPORT in $MEMPORTS
            do
                for FPALU in $FPALUS
                do
                    for FPMULT in $FPMULTS
                    do
                        for BPRED in $BPREDS
                        do
                            for ORDER in $ORDERS
                            do
                                echo "----------------------------------------"
                                echo
                                ./go.sh $WIDTH $INTALU $INTMULT $MEMPORT $FPALU $FPMULT $BPRED $ORDER
                            done
                        done
                    done
                done
            done
        done
    done
done

# footer
echo "----------------------------------------"
echo
echo "Total elapsed time: $(($SECONDS - START)) seconds"
echo
