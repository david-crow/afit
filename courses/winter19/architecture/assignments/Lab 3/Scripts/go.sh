#!/bin/bash

# program inputs
WIDTH=$1
INTALU=$2
INTMULT=$3
MEMPORT=$4
FPALU=$5
FPMULT=$6
B_PRED=$7
ORDER=$8

# file names
STDOUT=stdout/go_stdout_${WIDTH}_${INTALU}_${INTMULT}_${MEMPORT}_${FPALU}_${FPMULT}_${B_PRED}_${ORDER}.txt
OUTPUT=output/go_output_${WIDTH}_${INTALU}_${INTMULT}_${MEMPORT}_${FPALU}_${FPMULT}_${B_PRED}_${ORDER}.txt
DIFF=diff/go_diff_${WIDTH}_${INTALU}_${INTMULT}_${MEMPORT}_${FPALU}_${FPMULT}_${B_PRED}_${ORDER}.txt
INFILE=2stone9.in
OUTFILE=2stone9.out
TARGET=go.ss

# boolean representing whether or not the execution order is in-order
if [ $ORDER = "in-order" ]
then
    B_ORDER=true
else
    B_ORDER=false
fi

# start the timer
START=$SECONDS

# run SimpleScalar
echo "Starting go:"
echo "    machine width              : $WIDTH,"
echo "    integer ALUs               : $INTALU,"
echo "    integer multipliers        : $INTMULT,"
echo "    memory ports               : $MEMPORT,"
echo "    floating point ALUs        : $FPALU,"
echo "    floating point multipliers : $FPMULT,"
echo "    branch prediction          : $B_PRED,"
echo "    in-order execution         : $B_ORDER"
echo

sim-outorder \
    -fetch:ifqsize $WIDTH \
    -decode:width $WIDTH \
    -issue:width $WIDTH \
    -commit:width $WIDTH \
    -res:ialu $INTALU \
    -res:imult $INTMULT \
    -res:memport $MEMPORT \
    -res:fpalu $FPALU \
    -res:fpmult $FPMULT \
    -bpred $B_PRED \
    -issue:inorder $B_ORDER \
    -redir:sim "$STDOUT" \
    $TARGET \
    50 9 \
    $INFILE \
> $OUTPUT

echo "...done!"
echo

# ensure the output is correct
echo "Running diff..."
diff $OUTPUT $OUTFILE > $DIFF
echo "Results of diff:"
cat $DIFF

# footer
echo
echo "Elapsed time: $(($SECONDS - START)) seconds"
echo

cat $STDOUT | grep "CPI"
