#this is a batch making job designed to creat and submit batches of JDL jobs.

BATCH_DIR=$1
PPR_PREFIX=$4
START=$3
BIG_TOP=$4

for (( BIG_INDEX = $START ;  $BIG_INDEX < $BIG_TOP;  BIG_INDEX++  ))
do
 BOTTOM=`expr ${BIG_INDEX} "*" 10`
 TOP=`expr ${BOTTOM} + 10`
 SMALL_DIR=${BOTTOM}_${TOP}
 mkdir -p ${BATCH_DIR}/${SMALL_DIR}
 #./ittd/grid_cdm/scripts/delegation/delegation_2_0.sh
 for (( NAME_INDEX = ${BOTTOM} ;  $NAME_INDEX < $TOP;  NAME_INDEX++  ))
 do
 
  START_NUMBER=`expr ${NAME_INDEX} "*" 1000`
  END_NUMBER=`expr ${START_NUMBER} + 1000`
  FILE_NAME=${PPR_PREFIX}${START_NUMBER}_${END_NUMBER}
  cp ${BATCH_DIR}/${FILE_NAME}.jdl ${BATCH_DIR}/${SMALL_DIR}/${FILE_NAME}.jdl

 
 done

 
 
done
