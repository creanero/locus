#!/bin/bash

PPR_LOCATION=$1
exe_full_name=$2
COLOUR=$3
BATCH_NAME=$4

exe_short_name=`basename ${exe_full_name}`

#if [ ! -e ${PPR_LOCATION} ]; then
# echo ${PPR_LOCATION} does not exist
# exit 1
#fi

LFC_PPR_LOCATION=/grid/cosmo/${PPR_LOCATION}



JDL_LOCATION=`echo ${PPR_LOCATION} | sed 's/ppr/jdl/'`
echo ${JDL_LOCATION}

mkdir -p ${JDL_LOCATION}

umask 022
TMP_PPR_FILE=`mktemp /tmp/ittd_prm_listing.XXXXXXXXXX` || exit 1
lfc-ls ${LFC_PPR_LOCATION} | grep .ppr > ${TMP_PPR_FILE}
#ls -1 ${PPR_LOCATION}/*.ppr > ${TMP_PPR_FILE}

while read line 
do
 echo $line
 jdl_prefix=`basename $line .ppr`
 jdl_full_name="${JDL_LOCATION}/${jdl_prefix}.jdl"

 ppr_full_name="${PPR_LOCATION}/${jdl_prefix}.ppr"
 txt_full_name="${PPR_LOCATION}/${jdl_prefix}.txt"

 ppr_short_name="${jdl_prefix}.ppr"
 txt_short_name="${jdl_prefix}.txt"


cat >  ${jdl_full_name} <<EOF
Executable = "${exe_short_name}";
StdOutput = "std.out";
StdError = "std.err";
#Arguments = "${ppr_short_name} ${txt_short_name} ${COLOUR} ${BATCH_NAME}";
Arguments = "${ppr_full_name} ${txt_full_name} ${COLOUR} ${BATCH_NAME}";
OutputSandbox = {"std.out","std.err","logfile.out"};
InputSandbox = {"${exe_full_name}"};
#InputSandbox = {"${exe_full_name}","${ppr_full_name}", "${txt_full_name}"};
VirtualOrganisation = "cosmo";
Requirements = other.GlueCEUniqueID == "gridgate.scg.nuigalway.ie:2119/jobmanager-pbs-threeday" ||  other.GlueCEUniqueID == "gridgate.cs.tcd.ie:2119/jobmanager-pbs-threeday" || other.GlueCEUniqueID == "gridgate.ucc.ie:2119/jobmanager-pbs-threeday";
OutputSandbox = {"std.out","std.err","logfile.out"};

EOF
done < ${TMP_PPR_FILE}


