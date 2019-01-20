#!/bin/bash

#this script generates jdl jobs for the parameterisation stage of the XOP pahase of the project.

#this will be the folders the CSV files to be parameterised are in
CSV_LOCATION=$1
#this is the name of the parameterise executable script
exe_full_name=$2
#COLOUR=$3
#BATCH_NAME=$4

#these are the arguments from parameterisation_6_1
#All are commented as it is clearer to actually use the numbers and allow reference here if needed.
arg_num_targets=$3
prm_file_path=$4
#this one is commented out as it is redundant
#csv_file_name=$5
fit_file_path=$6
arg_fov_size=$7
arg_resolution=$8
arg_max_mag_diff=$9
arg_max_col_diff=${10}

#this one has been commented out as it is generated and used live
#xop_prm_file_name=$11




exe_short_name=`basename ${exe_full_name}`

if [ ! -e ${CSV_LOCATION} ]; then
 echo "${CSV_LOCATION} does not exist"
 exit 1
fi

if [ ! -e ${CSV_LOCATION}/small/ ]; then
 echo "${CSV_LOCATION}/small/ does not exist"
 exit 1
fi


#will put the jdls in a folder "beside" the CSV folder.  
#ensure that the folder name for the CSV files includes a directory layer called csv
JDL_LOCATION=`echo ${CSV_LOCATION} | sed 's/csv/jdl/'`
echo ${JDL_LOCATION}

mkdir -p ${JDL_LOCATION}

umask 022
TMP_CSV_FILE=`mktemp /tmp/ittd_csv_listing.XXXXXXXXXX` || exit 1
ls -1 ${CSV_LOCATION}/*.csv > ${TMP_CSV_FILE}

while read line 
do
 echo $line
 jdl_prefix=`basename $line .csv`
 jdl_full_name="${JDL_LOCATION}/${jdl_prefix}.jdl"

 csv_full_name="${CSV_LOCATION}/${jdl_prefix}.csv"
 csv_small_full_name="${CSV_LOCATION}/small/${jdl_prefix}_small.csv"

 csv_short_name="${jdl_prefix}.csv"
 csv_small_short_name="${jdl_prefix}_small.csv"

 unique_prm_file_path="${prm_file_path}/${jdl_prefix}"

 gzip -c ${csv_full_name} > ${csv_full_name}.gz

cat >  ${jdl_full_name} <<EOF
Executable = "${exe_short_name}";
StdOutput = "std.out";
StdError = "std.err";
#Arguments = "${csv_short_name} ${csv_small_short_name} $3 $4 $5 $6 $7 $8 $9 $10";
Arguments = "${csv_short_name} ${csv_small_short_name} $arg_num_targets $unique_prm_file_path $csv_short_name $fit_file_path $arg_fov_size $arg_resolution $arg_max_mag_diff $arg_max_col_diff";
OutputSandbox = {"std.out","std.err","logfile.out"};
InputSandbox = {"${exe_full_name}","${csv_full_name}.gz", "${csv_small_full_name}"};
VirtualOrganisation = "cosmo";
Requirements = other.GlueCEUniqueID == "gridgate.scg.nuigalway.ie:2119/jobmanager-pbs-threeday" ||  other.GlueCEUniqueID == "gridgate.cs.tcd.ie:2119/jobmanager-pbs-threeday" || other.GlueCEUniqueID == "gridgate.ucc.ie:2119/jobmanager-pbs-threeday";
OutputSandbox = {"std.out","std.err","logfile.out"};

EOF
done < ${TMP_CSV_FILE}


