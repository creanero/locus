#!/bin/bash -vx
#returned verbose command
#previously commented out the verbose command deliberately.

#this is a test script
#hello world file i/o test from grid
#20th April 2010
INFILE=$1
OUTFILE=$2

NUM_LINES=`wc $1 | awk '{print $1}'`

NAME=Stranger
P=p

echo "hello world file i/o test" > $OUTFILE

for (( NAME_INDEX = 1 ;  NAME_INDEX <= $NUM_LINES;  NAME_INDEX++  ))

do

NAME=`sed -n $NAME_INDEX$P $INFILE`

echo "Hello $NAME" >> $OUTFILE

done

exit 0
