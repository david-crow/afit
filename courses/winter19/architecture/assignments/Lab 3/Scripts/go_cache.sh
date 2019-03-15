#!/bin/bash

# program inputs
CSIZE=$1
BSIZE=$2
ASSOC=$3

# CSIZE in bytes = BSIZE * ASSOC * NSETS
# ∴ NSETS = CSIZE in bytes / (BSIZE * ASSOC)
NSET=$(($CSIZE * 1024 / $BSIZE / $ASSOC))

# file names
STDOUT=stdout/go_cache_stdout_${CSIZE}_${BSIZE}_${ASSOC}.txt
OUTPUT=output/go_cache_output_${CSIZE}_${BSIZE}_${ASSOC}.txt
DIFF=diff/go_cache_diff_${CSIZE}_${BSIZE}_${ASSOC}.txt
INFILE=2stone9.in
OUTFILE=2stone9.out
TARGET=go.ss

# start the timer
START=$SECONDS

# run SimpleScalar
echo "Starting go:"
echo "    cache size    : $CSIZE KB,"
echo "    block size    : $BSIZE B,"
echo "    associativity : $ASSOC-way"
echo

sim-outorder \
    -fetch:ifqsize 4 \
    -decode:width 4 \
    -issue:width 4 \
    -commit:width 4 \
    -res:ialu 1 \
    -res:imult 1 \
    -res:memport 1 \
    -res:fpalu 1 \
    -res:fpmult 1 \
    -bpred 2lev \
    -issue:inorder true \
    -cache:dl1 dl1:$NSET:$BSIZE:$ASSOC:l \
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
