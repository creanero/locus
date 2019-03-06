# This is a batch making job designed to go through and Check the status of batches of JDL jobs.

#the Barch dir is the directory to submit to the grid.
BATCH_DIR=$1
#the start number is the number of the count to start from
START=$2

BIG_TOP=$3
for (( BIG_INDEX = ${START} ;  $BIG_INDEX < $BIG_TOP;  BIG_INDEX++  ))
do
 BOTTOM=`expr ${BIG_INDEX} "*" 10`
 TOP=`expr ${BOTTOM} + 10`
 SMALL_DIR=${BOTTOM}_${TOP}

  echo "${SMALL_DIR}"
  glite-wms-job-status -i ${BATCH_DIR}/${SMALL_DIR}/joblist.out | grep  Status:



done
