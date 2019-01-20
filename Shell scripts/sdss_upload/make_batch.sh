BATCH_DIR=$1


BIG_TOP=43
for (( BIG_INDEX = 0 ;  $BIG_INDEX < $BIG_TOP;  BIG_INDEX++  ))
do
 BOTTOM=`expr ${BIG_INDEX} "*" 10`
 TOP=`expr ${BOTTOM} + 10`
 SMALL_DIR=${BOTTOM}_${TOP}
 mkdir ${BATCH_DIR}/${SMALL_DIR}
 ./ittd/grid_cdm/scripts/delegation/delegation_2_0.sh
 for (( NAME_INDEX = ${BOTTOM} ;  $NAME_INDEX < $TOP;  NAME_INDEX++  ))
 do
 
  START_NUMBER=`expr ${NAME_INDEX} "*" 1000`
  END_NUMBER=`expr ${START_NUMBER} + 1000`
  FILE_NAME=prm_11-5-2010_1000_${START_NUMBER}_${END_NUMBER}.txt
  cp ${BATCH_DIR}/${FILE_NAME} ${BATCH_DIR}/${SMALL_DIR}/${FILE_NAME}
 
 
 done

 ./ittd/grid_cdm/scripts/sdss_upload/outer_loop.sh ${BATCH_DIR}/${SMALL_DIR} ${BATCH_DIR}/${SMALL_DIR}/ver_${BOTTOM}_${TOP}.ver
 
done
