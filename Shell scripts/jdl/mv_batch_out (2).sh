#!/bin/bash -vx

# This is a batch making job designed to go through and submit batch`es of JDL jobs.

#the Barch dir is the directory to submit to the grid.
BATCH_DIR=$1
#the start number is the number of the count to start from
START=$2

BIG_TOP=$3
for (( BIG_INDEX = ${START} ;  $BIG_INDEX < $BIG_TOP;  BIG_INDEX++  ))
do
 BOTTOM=`expr ${BIG_INDEX} "*" 10`
 TOP=`expr ${BOTTOM} + 10`
 PREVIOUS=`expr ${BOTTOM} - 10`
 SMALL_DIR=${BOTTOM}_${TOP}

 #if the job is the first job, then submit it.

#if this is not the first directory

 mkdir -p ${BATCH_DIR}/old/${SMALL_DIR}/

 mv  ${BATCH_DIR}/${SMALL_DIR}/joblist.old  ${BATCH_DIR}/old/${SMALL_DIR}/joblist.out
 mv ${BATCH_DIR}/${SMALL_DIR}/old_output/ ${BATCH_DIR}/old/${SMALL_DIR}/output/
 


done
