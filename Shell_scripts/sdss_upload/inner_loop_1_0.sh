# /bin/bash

#this is a script to make a list of the files needed froma prm file using the 
#verification 4_1 script
#it then wgets those files to a temporary directory. 
#It then copies and registers the files in the LFC
#and finally removes the temporary directory.

#it has three inputs: the PRT file to be accessed. 
#the name of the lis file to be used in the wget and 
#the name of the directory to be used by wget

PRT_FILE=$1
LIS_FILE=$2
DIR_NAME=$3

~/ittd/grid_cdm/scripts/verification/verification_4_1.sh ${PRT_FILE} ${LIS_FILE}

mkdir -p ${DIR_NAME}

cd $DIR_NAME

cat ~/${LIS_FILE} | xargs -P 200 -r -n 1 wget --no-cache -c -nv

cd ~

~/ittd/grid_cdm/scripts/sdss_dl_cr/sdss_dl_cr_1_0.sh $PRT_FILE $DIR_NAME

rm ${DIR_NAME}/*.fit
rmdir ${DIR_NAME}

