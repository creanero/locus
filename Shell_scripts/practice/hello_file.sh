#!/bin/bash -vx
#returned verbose command
#previously commented out the verbose command deliberately.

#this is a test script
#hello world file input test from grid
#16th April 2010
FILENAME=$1

NUM_LINES=`wc $1 | awk '{print $1}'`

NAME=Stranger
P=p

for (( NAME_INDEX = 1 ;  NAME_INDEX <= $NUM_LINES;  NAME_INDEX++  ))

do

NAME=`sed -n $NAME_INDEX$P $FILENAME`

echo "Hello $NAME"
done

exit 0
