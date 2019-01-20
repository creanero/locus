#!/bin/bash

PRM_LOCATION=$1

if [ ! -e ${PRM_LOCATION} ]; then
 echo ${PRM_LOCATION} does not exist
 exit 1
fi

JDL_LOCATION=`echo ${PRM_LOCATION} | sed 's/prm/jdl/'`
echo ${JDL_LOCATION}

mkdir -p ${JDL_LOCATION}

umask 022
TMP_PRM_FILE=`mktemp /tmp/ittd_prm_listing.XXXXXXXXXX` || exit 1
ls -1 ${PRM_LOCATION}/*.prm > ${TMP_PRM_FILE}

while read line 
do
 echo $line
 jdl_prefix=`basename $line .prm`
 jdl_full_name="${JDL_LOCATION}/${jdl_prefix}.jdl"

 prm_full_name="${PRM_LOCATION}/${jdl_prefix}.prm"
 txt_full_name="${PRM_LOCATION}/${jdl_prefix}.txt"

 prm_short_name="${jdl_prefix}.prm"
 txt_short_name="${jdl_prefix}.txt"

cat >  ${jdl_full_name} <<EOF
Executable = "call_api_3_0.sh";
StdOutput = "std.out";
StdError = "std.err";
Arguments = "${prm_short_name} ${txt_short_name}";
#Arguments = "${prm_full_name} ${txt_full_name}";
OutputSandbox = {"std.out","std.err"};
InputSandbox = {"call_api_3_0.sh","${prm_full_name}", "${txt_full_name}"};
VirtualOrganisation = "cosmo";
EOF
done < ${TMP_PRM_FILE}
