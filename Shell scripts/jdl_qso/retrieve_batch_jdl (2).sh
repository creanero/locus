# This is a batch making job designed to go through and retrieve the ourput of batches of JDL jobs.

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


  #renew delegation every half an hour
  ./ittd/grid_cdm/scripts/delegation/delegation_2_0.sh

  echo "${SMALL_DIR}"
  glite-wms-job-output --noint -i ${BATCH_DIR}/${SMALL_DIR}/joblist.out --dir ${BATCH_DIR}/${SMALL_DIR}/output/
   
   #remives the std.out and std.err files
   find ${BATCH_DIR}/${SMALL_DIR}/output/ -name 'std.out' -exec rm {} \;
   find ${BATCH_DIR}/${SMALL_DIR}/output/ -name 'std.err' -exec rm {} \;


done
