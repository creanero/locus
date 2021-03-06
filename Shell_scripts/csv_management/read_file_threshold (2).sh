#!/bin/bash

TMP=$1


big_start_field_id=$2

big_end_field_id=$3

num_fields=$4

threshold=$5

num_files=0

split_files_name=`basename ${TMP} .csv`
orig_dir=`dirname ${TMP}`
split_files_dir="${orig_dir}/split"
mkdir -p $split_files_dir
outfile="${split_files_dir}/${split_files_name}_${num_files}.csv"

previous_field=$big_start_field_id
current_field_count=0

BAKIFS=$IFS
IFS=$(echo -en "\n\b")
exec 3<&0
exec 0<"$TMP"
read -r line
header_line=$line
#   echo "$header_line"
#exit 0


mkdir -p ${split_files_dir}/over_threshold/
over_threshold_file="${split_files_dir}/over_threshold/${split_files_name}_over_threshold.csv"

echo ${header_line} > ${over_threshold_file}

echo ${header_line} > ${outfile}

while read -r line
do

  current_field=`echo $line | awk -F, '{print $2'}` 
  #if after the end of the range specified, stop and exit
  if [ $current_field -gt $big_end_field_id ]; then
    exit 0
  fi
 
  #if before the start of the range specified, do nothing
  if [ $current_field -lt $big_start_field_id ]; then
   #do nothing
   echo ""

  #if within the range continue the process
  else
   #if above the threshold, divert to overflow file
   current_num_fields=`echo $line | awk -F, '{print $1'}`
   if [ $current_num_fields -gt $threshold ]; then
      ( echo ${line} ) >> ${over_threshold_file}
   else


   if [ $current_field -gt $previous_field ]; then
    current_field_count=$((current_field_count + 1))
    if [ $current_field_count -gt $num_fields ]; then
     num_files=$((num_files + 1))
     echo 
     outfile="${split_files_dir}/${split_files_name}_${num_files}.csv"
     echo $outfile
     echo "$header_line" > $outfile
     current_field_count=1
    fi
    previous_field=$current_field
   fi 
  # echo "$line" 2>&1 >> "${out_file}"
   ( echo ${line} ) >> $outfile
  fi
 fi
done
IFS=$BAKIFS

