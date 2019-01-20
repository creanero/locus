#!/bin/bash

EXECUTABLE_NAME=$1
PRT_LOCATION=$2
#VER_OVERALL=$3

if [ ! -e ${PRT_LOCATION} ]; then
 echo ${PRT_LOCATION} does not exist
 exit 1
fi

JDL_LOCATION=`echo ${PRT_LOCATION} | sed 's/prm/dl_jdl/'`
echo ${JDL_LOCATION}

mkdir -p ${JDL_LOCATION}

umask 022
TMP_PRT_FILE=`mktemp /tmp/ittd_prm_listing.XXXXXXXXXX` || exit 1
ls -1 ${PRT_LOCATION}/*.txt > ${TMP_PRT_FILE}

while read line 
do
 echo $line
 jdl_prefix=`basename $line .txt`
 jdl_full_name="${JDL_LOCATION}/${jdl_prefix}.jdl"

 txt_full_name="${PRT_LOCATION}/${jdl_prefix}.txt"

 txt_short_name="${jdl_prefix}.txt"
 VER_OVERALL="${jdl_prefix}.ver"

cat >  ${jdl_full_name} <<EOF
Executable = "${EXECUTABLE_NAME}";
StdOutput = "std.out";
StdError = "std.err";
Arguments = "\`pwd\` ${VER_OVERALL}";
OutputSandbox = {"std.out","std.err", "${VER_OVERALL}"};
InputSandbox = {"${EXECUTABLE_NAME}", "${txt_full_name}"};
Requirements = other.GlueCEUniqueID == "gridgate.ucc.ie:2119/jobmanager-pbs-threeday";
VirtualOrganisation = "cosmo";
EOF
done < ${TMP_PRT_FILE}
