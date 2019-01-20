#!/bin/bash

JDL_LOCATION=$1

if [ ! -e ${JDL_LOCATION} ]; then
 echo ${JDL_LOCATION} does not exist
 exit 1
fi


umask 022
TMP_JDL_FILE=`mktemp /tmp/ittd_jdl_listing.XXXXXXXXXX` || exit 1
ls -1 ${JDL_LOCATION}/*.jdl > ${TMP_JDL_FILE}

while read line 
do
 echo $line

done < ${TMP_JDL_FILE}
