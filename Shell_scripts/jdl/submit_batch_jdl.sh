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
 PREV_DIR=${PREVIOUS}_${BOTTOM}
 SUBMITTED=0

 #if the job is the first job, then submit it.
 if [ ${PREVIOUS} -lt ${START} ]
 then
 glite-wms-job-submit -a -o ${BATCH_DIR}/${SMALL_DIR}/joblist.out --collection ${BATCH_DIR}/${SMALL_DIR}

 fi

#if this is not the first directory
 if [ ${PREVIOUS} -ge ${START} ]
 then

#then carry out the rest of the loop
#while the current directory has not been sumbitted
 while [ "${SUBMITTED}" -ne 1 ]
 do
  #renew delegation every half an hour
  ./ittd/grid_cdm/scripts/delegation/delegation_2_0.sh

  #count if the previous job is all running
  NOT_RUNNING_COUNT=0

  SUBMITTED_COUNT=`glite-wms-job-status -i ${BATCH_DIR}/${PREV_DIR}/joblist.out | grep -c Submitted`
  NOT_RUNNING_COUNT=`expr ${NOT_RUNNING_COUNT} + ${SUBMITTED_COUNT}`

  WAITING_COUNT=`glite-wms-job-status -i ${BATCH_DIR}/${PREV_DIR}/joblist.out | grep -c Waiting`
  NOT_RUNNING_COUNT=`expr ${NOT_RUNNING_COUNT} + ${WAITING_COUNT}`

  READY_COUNT=`glite-wms-job-status -i ${BATCH_DIR}/${PREV_DIR}/joblist.out | grep -c Ready`
  NOT_RUNNING_COUNT=`expr ${NOT_RUNNING_COUNT} + ${READY_COUNT}`

  SCHEDULED_COUNT=`glite-wms-job-status -i ${BATCH_DIR}/${PREV_DIR}/joblist.out | grep -c Scheduled`
  NOT_RUNNING_COUNT=`expr ${NOT_RUNNING_COUNT} + ${SCHEDULED_COUNT}`

  #if it is, submit the next job
  if [ ${NOT_RUNNING_COUNT} -eq 0 ]
  then
   glite-wms-job-submit -a -o ${BATCH_DIR}/${SMALL_DIR}/joblist.out --collection ${BATCH_DIR}/${SMALL_DIR}

  #and flg that the job has been sumbitted
  SUBMITTED=1
  fi
  #then wait half an hour
  sleep 300;
  

 done
 fi

done
